
#include <Bg2ModelExport.hpp>

#include <json.hpp>
#include <json-parser.hpp>
#include <bg2-material.hpp>

#include <iostream>
#include <filesystem>
#include <fstream>

Bg2ModelExport::Bg2ModelExport(tinygltf::Model& model, const std::string& imageInputPath, const std::string& imageOutputPath)
    :_imageInputPath{imageInputPath}
    ,_imageOutputPath{imageOutputPath}
    ,_model{&model}
    ,_meshIndex(-1)
{
}

int Bg2ModelExport::addMesh(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& texCoord0,
	const std::vector<uint32_t>& indices,
    int materialIndex
)
{
    int baseBufferIndex = static_cast<int>(_model->buffers.size());
    int baseBufferViewIndex = static_cast<int>(_model->bufferViews.size());
    int baseAccessorIndex = static_cast<int>(_model->accessors.size());

    tinygltf::Buffer buffer;
    tinygltf::BufferView indexBufferView;
    tinygltf::BufferView attributeBufferView;
    tinygltf::Accessor indexAccessor;
    tinygltf::Accessor positionAccessor;
    tinygltf::Accessor normalAccessor;
    tinygltf::Accessor tex0Accessor;

    // Index data
    uint32_t maxIndex = std::numeric_limits<uint32_t>::min();
    uint32_t minIndex = std::numeric_limits<uint32_t>::max();
    for (auto i = 0; i < indices.size(); ++i)
    {
        auto index = indices[i];
        writeBuffer(index, buffer.data);
        if (index > maxIndex)
        {
            maxIndex = index;
        }
        if (index < minIndex)
        {
            minIndex = index;
        }
    }

    indexBufferView.buffer = baseBufferIndex;
    indexBufferView.byteOffset = 0;
    indexBufferView.byteLength = buffer.data.size();
    indexBufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    indexAccessor.bufferView = baseBufferViewIndex;
    indexAccessor.byteOffset = 0;
    indexAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
    indexAccessor.count = indices.size();
    indexAccessor.type = TINYGLTF_TYPE_SCALAR;
    indexAccessor.maxValues.push_back(maxIndex);
    indexAccessor.minValues.push_back(minIndex);

    // Padding
    size_t offset = buffer.data.size();
    while (buffer.data.size() % sizeof(float) != 0)
    {
        buffer.data.push_back(0);
        ++offset;
    }

    // Vertex data
    std::array<float, 3> maxPositions = 
    {
       -std::numeric_limits<float>::max(),
       -std::numeric_limits<float>::max(),
       -std::numeric_limits<float>::max()
    };
    std::array<float, 3> minPositions = 
    {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    };
    std::array<float, 3> maxNormal = 
    {
        -std::numeric_limits<float>::max(),
        -std::numeric_limits<float>::max(),
        -std::numeric_limits<float>::max()
    };
    std::array<float, 3> minNormal = 
    {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    };
    std::array<float, 2> maxTex0 = 
    {
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::min()
    };
    std::array<float, 2> minTex0 = 
    {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    };
    size_t numVertex = positions.size() / 3;
    size_t bufferLength = 0;
    for (auto i = 0; i < numVertex; ++i) 
    {
        auto offset3 = i * 3;
        auto offset2 = i * 2; // Para coordenadas de textura
        auto x = positions[offset3];
        auto y = positions[offset3 + 1];
        auto z = positions[offset3 + 2];
        writeBuffer(x, buffer.data);
        writeBuffer(y, buffer.data);
        writeBuffer(z, buffer.data);
        updateRange(x, y, z, maxPositions, minPositions);
        bufferLength += sizeof(float) * 3;

        // Escribir las normales (entrelazado)
        auto nx = normals[offset3];
        auto ny = normals[offset3 + 1];
        auto nz = normals[offset3 + 2];
        auto length = sqrt(nx * nx + ny * ny + nz * nz);
        nx /= length;
        ny /= length;
        nz /= length;
        writeBuffer(nx, buffer.data);
        writeBuffer(ny, buffer.data);
        writeBuffer(nz, buffer.data);
        updateRange(nx, ny, nz, maxNormal, minNormal);
        bufferLength += sizeof(float) * 3;

        auto t0x = texCoord0[offset2];
        auto t0y = texCoord0[offset2 + 1];
        writeBuffer(t0x, buffer.data);
        writeBuffer(t0y, buffer.data);
        updateRange(t0x, t0y, maxTex0, minTex0);
        bufferLength += sizeof(float) * 2;
    }

    attributeBufferView.buffer = baseBufferIndex;
    attributeBufferView.byteOffset = offset;
    attributeBufferView.byteLength = bufferLength;
    attributeBufferView.byteStride =
        sizeof(float) * 3 +     // Posiciones
        sizeof(float) * 3 +     // Normales
        sizeof(float) * 2;      // Coord Textura 0
    attributeBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

    positionAccessor.bufferView = baseBufferViewIndex + 1;
    positionAccessor.byteOffset = 0;
    positionAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    positionAccessor.count = positions.size() / 3;
    positionAccessor.type = TINYGLTF_TYPE_VEC3;
    positionAccessor.maxValues = { maxPositions[0], maxPositions[1], maxPositions[2] };
    positionAccessor.minValues = { minPositions[0], minPositions[1], minPositions[2] };

    normalAccessor.bufferView = baseBufferViewIndex + 1;
    normalAccessor.byteOffset = sizeof(float) * 3;  // 3 floats antes de llegar a las normales
    normalAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    normalAccessor.count = normals.size() / 3;
    normalAccessor.type = TINYGLTF_TYPE_VEC3;
    normalAccessor.maxValues = { maxNormal[0], maxNormal[1], maxNormal[2] };
    normalAccessor.minValues = { minNormal[0], minNormal[1], minNormal[2] };

    tex0Accessor.bufferView = baseBufferViewIndex + 1;
    tex0Accessor.byteOffset = sizeof(float) * 6; // 6 floats antes de llegar a texcoord0
    tex0Accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    tex0Accessor.count = texCoord0.size() / 2;
    tex0Accessor.type = TINYGLTF_TYPE_VEC2;
    tex0Accessor.maxValues = { maxTex0[0], maxTex0[1] };
    tex0Accessor.minValues = { minTex0[0], minTex0[1] };

    tinygltf::Primitive primitive;
    primitive.indices = baseAccessorIndex;
    primitive.attributes["POSITION"] = baseAccessorIndex + 1;
    primitive.attributes["NORMAL"] = baseAccessorIndex + 2;
    primitive.attributes["TEXCOORD_0"] = baseAccessorIndex + 3;
    primitive.material = materialIndex;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;

    _model->buffers.push_back(buffer);
    _model->bufferViews.push_back(indexBufferView);
    _model->bufferViews.push_back(attributeBufferView);
    _model->accessors.push_back(indexAccessor);
    _model->accessors.push_back(positionAccessor);
    _model->accessors.push_back(normalAccessor);
    _model->accessors.push_back(tex0Accessor);

    // Add primitive to mesh
    _mesh.primitives.push_back(primitive);
    _meshIndex = static_cast<int>(_model->meshes.size());
    _model->meshes.push_back(_mesh);
    
    return _meshIndex;
}

int Bg2ModelExport::addBg2Model(Bg2FileReader& bg2Reader)
{
    int meshIndex = -1;

    // Create scene if not exist
    if (_model->scenes.size() == 0) 
    {
        _model->scenes.push_back(tinygltf::Scene());
    }
    // The models are always added to the first scene
    tinygltf::Scene& scene = _model->scenes[0];

    auto file = bg2Reader.bg2File();
 
    bg2scene::json::JsonParser parser(file->materialData);
    auto rootNode = parser.parse();
    std::map<std::string, bg2scene::Bg2Material> materials;
    if (rootNode->isList())
    {
        auto materialList = rootNode->listValue();
        for (auto item : materialList)
        {
            if (item->isObject())
            {
                bg2scene::Bg2Material mat(item->objectValue());
                materials[mat.name] = mat;                
            }
        }
    }

    
    // PolyList
    for (int i = 0; i < file->plists->length; ++i)
    {
        Bg2ioPolyList* plist = file->plists->data[i];
        bool complete = 
            plist->vertex.data != nullptr &&
            plist->normal.data != nullptr &&
            plist->texCoord0.data != nullptr &&
            plist->index.data != nullptr;
         
        if (complete)
        {
            std::vector<float> positions{ plist->vertex.data, plist->vertex.data + plist->vertex.length };
            std::vector<float> normals{ plist->normal.data, plist->normal.data + plist->normal.length };
            std::vector<float> texCoord0{ plist->texCoord0.data, plist->texCoord0.data + plist->texCoord0.length };
            std::vector<uint32_t> index{ plist->index.data, plist->index.data + plist->index.length };
            // TODO: rest of the texCoordN, if available

            tinygltf::Material mat;
            if (materials.find(plist->matName) != materials.end())
            {
                auto bg2Mat = materials[plist->matName];
                
                if (bg2Mat.diffuseTexture != "")
                {
                    mat.pbrMetallicRoughness.baseColorTexture.index = getTexture(bg2Mat.diffuseTexture);
                    auto textureTransform = getTextureTransform(
                        bg2Mat.diffuseOffset[0], bg2Mat.diffuseOffset[1],
                        bg2Mat.diffuseScale[0], bg2Mat.diffuseScale[1]);
                    mat.pbrMetallicRoughness.baseColorTexture.extensions["KHR_texture_transform"] = textureTransform;
                }
                mat.pbrMetallicRoughness.metallicFactor = bg2Mat.metallic;
                mat.pbrMetallicRoughness.roughnessFactor = bg2Mat.roughness;

                if (bg2Mat.normalTexture != "")
                {
                    mat.normalTexture.index = getTexture(bg2Mat.normalTexture);
                    auto textureTransform = getTextureTransform(
                        bg2Mat.normalOffset[0], bg2Mat.normalOffset[1],
                        bg2Mat.normalScale[0], bg2Mat.normalScale[1]);
                    mat.normalTexture.extensions["KHR_texture_transform"] = textureTransform;
                }

                // TODO: Rest of material properties
            }
            else
            {
                mat.pbrMetallicRoughness.baseColorFactor = { 1.f, 1.f, 1.f, 1.f };
            }
            auto matIndex = static_cast<int>(_model->materials.size());
            _model->materials.push_back(mat);
            

            meshIndex = addMesh(positions, normals, texCoord0, index, matIndex);

            // Create a new node for the mesh
            tinygltf::Node node;
            node.mesh = meshIndex;
            _lastNodeIndex = static_cast<int>(scene.nodes.size());
            scene.nodes.push_back(static_cast<int>(scene.nodes.size()));
            _model->nodes.push_back(node);
        }
        else
        {
            // TODO: Warning: plist not complete
        }
        
        
    }

    // Return last mesh index, or -1 if no mesh has been added
    return meshIndex;
}


void Bg2ModelExport::writeBuffer(float value, std::vector<uint8_t>& buffer)
{
    union {
        float number;
        uint8_t bytes[4];
    } bufferData;
    bufferData.number = value;
    buffer.push_back(bufferData.bytes[0]);
    buffer.push_back(bufferData.bytes[1]);
    buffer.push_back(bufferData.bytes[2]);
    buffer.push_back(bufferData.bytes[3]);
}

void Bg2ModelExport::writeBuffer(uint32_t value, std::vector<uint8_t>& buffer)
{
    union {
        uint32_t value;
        uint8_t bytes[4];
    } bufferData;
    bufferData.value = value;
    buffer.push_back(bufferData.bytes[0]);
    buffer.push_back(bufferData.bytes[1]);
    buffer.push_back(bufferData.bytes[2]);
    buffer.push_back(bufferData.bytes[3]);
}

void Bg2ModelExport::writeBuffer(uint16_t value, std::vector<uint8_t>& buffer)
{
    union {
        uint16_t value;
        uint8_t bytes[2];
    } bufferData;
    bufferData.value = value;
    buffer.push_back(bufferData.bytes[0]);
    buffer.push_back(bufferData.bytes[1]);
}

void Bg2ModelExport::updateRange(float x, float y, float z, std::array<float, 3>& max, std::array<float, 3>& min)
{
    if (max[0] < x) 
    {
        max[0] = x;
    }
    if (min[0] > x) 
    {
        min[0] = x;
    }
    if (max[1] < y) 
    {
        max[1] = y;
    }
    if (min[1] > y) 
    {
        min[1] = y;
    }
    if (max[2] < z) 
    {
        max[2] = z;
    }
    if (min[2] > z) 
    {
        min[2] = z;
    }
}

void Bg2ModelExport::updateRange(float x, float y, std::array<float, 2>& max, std::array<float, 2>& min)
{
    if (max[0] < x) 
    {
        max[0] = x;
    }
    if (min[0] > x) 
    {
        min[0] = x;
    }
    if (max[1] < y) 
    {
        max[1] = y;
    }
    if (min[1] > y) 
    {
        min[1] = y;
    }
}

int Bg2ModelExport::getTexture(const std::string& imagePath)
{
    if (_textures.find(imagePath) != _textures.end())
    {
        return _textures[imagePath];
    }
    else
    {
        tinygltf::Texture texture;
        texture.sampler = getSampler();
        texture.source = getImageIndex(imagePath);
        auto texIndex = static_cast<int>(_model->textures.size());
        _model->textures.push_back(texture);
        _textures[imagePath] = texIndex;
        return texIndex;
    }
}

int Bg2ModelExport::getImageIndex(const std::string& imagePath)
{
    if (_images.find(imagePath) != _images.end())
    {
        return _images[imagePath];
    }
    else
    {
        tinygltf::Image img;
        img.uri = imagePath;
        auto imgIndex = static_cast<int>(_model->images.size());
        _model->images.push_back(img);
        _images[imagePath] = imgIndex;

        // Copy image to output path, if the file does not exists
        namespace fs = std::filesystem;
        auto dstImagePath = fs::path(_imageOutputPath) / fs::path(imagePath);
        std::ifstream file(dstImagePath.string());
        if (!file.good())
        {
            auto srcImagePath = fs::path(_imageInputPath) / fs::path(imagePath);
            // TODO: copy image
            std::ifstream inFile(srcImagePath.string(), std::ios::binary);
            std::ofstream outFile(dstImagePath.string(), std::ios::binary);
            if (!inFile.good())
            {
                std::cout << "Warning: no such image at path '" << srcImagePath << "'" << std::endl;
            }

            if (!outFile.good())
            {
                std::cout << "Warning: could not open file for write '" << dstImagePath << "'" << std::endl;
            }

            outFile << inFile.rdbuf();
            outFile.close();

            if (inFile.good())
            {
                inFile.close();
            }
            if (outFile.good())
            {
                outFile.close();
            }
        }

        return imgIndex;
    }
}

int Bg2ModelExport::getSampler()
{
    tinygltf::Sampler sampler;
    sampler.magFilter = TINYGLTF_TEXTURE_FILTER_LINEAR;
    sampler.minFilter = TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR;
    sampler.wrapS = TINYGLTF_TEXTURE_WRAP_REPEAT;
    sampler.wrapT = TINYGLTF_TEXTURE_WRAP_REPEAT;
    _model->samplers.push_back(sampler);
    return 0;
}

tinygltf::Value Bg2ModelExport::getTextureTransform(float offsetX, float offsetY, float scaleX, float scaleY)
{
    tinygltf::Value::Object textureTransform;
    tinygltf::Value::Array offset;
    textureTransform["scale"] = tinygltf::Value(tinygltf::Value::Array{
        tinygltf::Value(scaleX),
        tinygltf::Value(scaleY)
    });
    textureTransform["offset"] = tinygltf::Value(tinygltf::Value::Array{
        tinygltf::Value(offsetX),
        tinygltf::Value(offsetY)
    });
    return tinygltf::Value(textureTransform);
}
