
#include <Bg2SceneExport.hpp>

#include <fstream>
#include <sstream>

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
        std::stringstream streamBuffer;
        streamBuffer << file.rdbuf();
        file.close();
        bg2scene::json::JsonParser parser(&streamBuffer);
        auto rootNode = parser.parse();
        if (rootNode && rootNode->objectValue("scene").isList()) {
            auto scene = rootNode->objectValue("scene").listValue();
            for (auto node : scene)
            {
                if (node->isObject())
                {
                    parseNode(node->objectValue());
                }
            }
        }

    }
    else
    {
        throw std::runtime_error("Could not open input scene file");
    }
}

void Bg2SceneExport::parseNode(bg2scene::json::JsonObject& node)
{
    auto nodeName = node["name"]->stringValue("");
    std::cout << nodeName << std::endl;
    if (node["components"]->isObject())
    {
    
    }
    if (node["children"]->isList())
    {
        for (auto child : node["children"]->listValue())
        {
            if (child->isObject())
            {
                parseNode(child->objectValue());
            }
        }
    }
}