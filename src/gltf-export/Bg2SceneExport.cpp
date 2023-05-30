
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
            glm::mat4 worldTransform(1.0f);
            auto scene = rootNode->objectValue("scene").listValue();
            for (auto node : scene)
            {
                if (node->isObject())
                {
                    parseNode(node->objectValue(), worldTransform);
                }
            }
        }

    }
    else
    {
        throw std::runtime_error("Could not open input scene file");
    }
}

void Bg2SceneExport::parseNode(bg2scene::json::JsonObject& node, const glm::mat4 & transform)
{
    auto nodeName = node["name"]->stringValue("");
    std::cout << nodeName << std::endl;
    glm::mat4 nodeTransform = transform;
    if (node["components"]->isList())
    {
        for (const auto& comp : node["components"]->listValue())
        {
            if (comp->isObject())
            {
                const auto& type = comp->objectValue("type").stringValue("");
                std::cout << "component type: " << type << std::endl;
                
                if (type == "Transform")
                {
                    auto m = comp->objectValue("transformMatrix").mat4Value({
                        1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 1.f
                    });
                    glm::mat4 trx{
                        m[0], m[1], m[2], m[3],
                        m[4], m[5], m[6], m[7],
                        m[8], m[9], m[10], m[11],
                        m[12], m[13], m[14], m[15]
                    };
                    nodeTransform = nodeTransform * trx;
                }
            }
        }
        
        // TODO: If there is some drawable node, add a node to the scene with the accumulated transform value and the mesh
    }
    if (node["children"]->isList())
    {
        for (const auto& child : node["children"]->listValue())
        {
            if (child->isObject())
            {
                parseNode(child->objectValue(), transform);
            }
        }
    }
}
