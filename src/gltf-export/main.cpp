#include <iostream>

#include <Bg2ModelExport.hpp>
#include <Bg2FileReader.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

void exportFile(const std::string& inFilePath, const std::string& outFilePath);

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage:" << std::endl << "gltf-export modelFile.bg2 modelFile.gltf" << std::endl;
        return 1;
    }

    try {
        std::string inFile = argv[1];
        std::string outFile = argv[2];
        exportFile(inFile, outFile);
    }
    catch (std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        return 2;
    }

    return 0;
}

void exportFile(const std::string& inFilePath, const std::string& outFilePath)
{
    Bg2FileReader bg2File;

    bg2File.open(inFilePath);

    tinygltf::Model m;
    tinygltf::Scene scene;
    m.scenes.push_back(scene);

    tinygltf::Asset asset;
    asset.version = "2.0";
    asset.generator = "tinygltf";
    m.asset = asset;

    Bg2ModelExport modelExport(m);
    modelExport.addBg2Model(bg2File);

    tinygltf::TinyGLTF gltf;
    gltf.WriteGltfSceneToFile(&m, outFilePath,
        true,
        true,
        true,
        false);
}