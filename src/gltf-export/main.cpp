#include <iostream>

#include <Bg2ModelExport.hpp>
#include <Bg2FileReader.hpp>
#include <Bg2SceneExport.hpp>
#include <filesystem>

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

void exportFile(const std::string& inFile, const std::string& outFile)
{
    tinygltf::Model m;
    tinygltf::Scene scene;
    m.scenes.push_back(scene);

    tinygltf::Asset asset;
    asset.version = "2.0";
    asset.generator = "tinygltf";
    m.asset = asset;

    m.extensionsUsed.push_back("KHR_texture_transform");

    std::filesystem::path inFilePath = inFile;
    std::filesystem::path outFilePath = outFile;
    auto imageInputDir = inFilePath.parent_path().string();
    auto imageOutputDir = outFilePath.parent_path().string();
    if (inFilePath.extension() == ".bg2" || inFilePath.extension() == ".vwglb")
    {
        Bg2FileReader bg2File;
        bg2File.open(inFile);
        Bg2ModelExport modelExport(m, imageInputDir, imageOutputDir);
        modelExport.addBg2Model(bg2File);
    }
    else if (inFilePath.extension() == ".vitscnj" || inFilePath.extension() == ".json")
    {
        Bg2SceneExport sceneExport(m);
        sceneExport.addSceneFile(inFile, imageOutputDir);
    }

    tinygltf::TinyGLTF gltf;
    gltf.WriteGltfSceneToFile(&m, outFile, true, true, true, false);
}