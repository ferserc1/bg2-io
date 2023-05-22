#include <iostream>

#include <Bg2ModelExport.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>



int main(int argc, char** argv)
{
    tinygltf::Model m;
    tinygltf::Scene scene;
    tinygltf::Node node;
    tinygltf::Asset asset;

    // Geometry data
    std::vector<uint32_t> index = {
        0, 1, 2,
        2, 3, 0
    };
    std::vector<float> positions = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    std::vector<float> normals = {
        .0f, .0f, 1.0f,
        .0f, .0f, 1.0f,
        .0f, .0f, 1.0f,
        .0f, .0f, 1.0f
    };
    std::vector<float> texCoords = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Material data
    tinygltf::Image image;
    image.uri = "testTexture.png";
    m.images.push_back(image);

    tinygltf::Sampler sampler;
    sampler.magFilter = TINYGLTF_TEXTURE_FILTER_LINEAR;
    sampler.minFilter = TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR;
    sampler.wrapS = TINYGLTF_TEXTURE_WRAP_REPEAT;
    sampler.wrapT = TINYGLTF_TEXTURE_WRAP_REPEAT;
    m.samplers.push_back(sampler);

    tinygltf::Texture tex;
    tex.sampler = 0; // Sampler at 0 position in m.samplers
    tex.source = 0;  // Image at 0 index in m.images
    m.textures.push_back(tex);

    tinygltf::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = { 1.f, 1.f, 1.f, 1.f };
    mat.pbrMetallicRoughness.baseColorTexture.index = 0;    // Texture at 0 index in m.textures
    mat.pbrMetallicRoughness.metallicFactor = 1.f;
    mat.pbrMetallicRoughness.roughnessFactor = 0.1f;
    mat.pbrMetallicRoughness.metallicRoughnessTexture.index = 0;
    mat.doubleSided = true;
    m.materials.push_back(mat);

    Bg2ModelExport modelExport(m);
    auto meshIndex = modelExport.addMesh(
        positions,
        normals,
        texCoords,
        index,
        0   // Material at index 0 in m.materials
    );

    node.mesh = meshIndex;
    scene.nodes.push_back(0);

    asset.version = "2.0";
    asset.generator = "tinygltf";

    m.scenes.push_back(scene);
    m.nodes.push_back(node);
    m.asset = asset;

    tinygltf::TinyGLTF gltf;
    gltf.WriteGltfSceneToFile(&m, "quad.gltf",
        true,
        true,
        true,
        false);

    
    return 0;
}