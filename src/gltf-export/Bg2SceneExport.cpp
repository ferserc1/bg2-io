
#include <Bg2SceneExport.hpp>

#include <fstream>

#include <json.hpp>
#include <json-parser.hpp>

Bg2SceneExport::Bg2SceneExport(tinygltf::Model& model)
    :_model(&model)
{

}

void Bg2SceneExport::addSceneFile(const std::string& scenePath)
{
    std::ifstream file;
    file.open(scenePath);
    if (file.is_open())
    {
        bg2scene::json::JsonParser parser(&file);
        auto rootNode = parser.parse();


    }
    else
    {
        throw std::runtime_error("Could not open input scene file");
    }
}

