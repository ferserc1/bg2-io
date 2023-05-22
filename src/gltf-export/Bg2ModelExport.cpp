
#include <Bg2ModelExport.hpp>

Bg2ModelExport::Bg2ModelExport(tinygltf::Model& model)
	:_model{&model}
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
    if (_meshIndex == -1)
    {
        _meshIndex = static_cast<int>(_model->meshes.size());
        _model->meshes.push_back(_mesh);
    }

    return _meshIndex;
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
