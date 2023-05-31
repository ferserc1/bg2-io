
#include <Bg2SceneExport.hpp>
#include <Bg2ModelExport.hpp>
#include <Bg2FileReader.hpp>

#include <fstream>
#include <sstream>
#include <filesystem>

#include <json.hpp>
#include <json-parser.hpp>

Bg2SceneExport::Bg2SceneExport(tinygltf::Model& model)
    :_model(&model)
{

}

void Bg2SceneExport::addSceneFile(const std::string& scenePath, const std::string& outPath)
{
    std::ifstream file;
    file.open(scenePath);
    if (file.is_open())
    {
        namespace fs = std::filesystem;
        fs::path fullScenePath(scenePath);
        std::string basePath = fullScenePath.parent_path().string();
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
                    parseNode(node->objectValue(), worldTransform, basePath, outPath);
                }
            }
        }

    }
    else
    {
        throw std::runtime_error("Could not open input scene file");
    }
}

void Bg2SceneExport::parseNode(bg2scene::json::JsonObject& node, const glm::mat4 & transform, const std::string& basePath, const std::string& outPath)
{
    auto nodeName = node["name"]->stringValue("");
    glm::mat4 nodeTransform = transform;
    std::string drawableName = "";
    if (node["components"]->isList())
    {
        for (const auto& comp : node["components"]->listValue())
        {
            if (comp->isObject())
            {
                const auto& type = comp->objectValue("type").stringValue("");
                
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

                if (type == "Drawable")
                {
                    drawableName = comp->objectValue("name").stringValue("");
                }
            }
        }
        
        if (drawableName != "")
        {
            namespace fs = std::filesystem;
            fs::path drawablePath = fs::path(basePath) / drawableName;
            drawablePath.replace_extension("bg2");
            if (!fs::exists(drawablePath)) {
                drawablePath.replace_extension("vwglb");
            }

            if (fs::exists(drawablePath)) {
                Bg2FileReader bg2File;
                bg2File.open(drawablePath.string());
                Bg2ModelExport modelExport(*_model, drawablePath.parent_path().string(), outPath);
                modelExport.addBg2Model(bg2File);
                auto nodeIndex = modelExport.getLastNodeIndex();
                if (nodeIndex >= 0)
                {
                    auto &node = _model->nodes[nodeIndex];
                    node.matrix = {
                        nodeTransform[0][0], nodeTransform[0][1], nodeTransform[0][2], nodeTransform[0][3],
                        nodeTransform[1][0], nodeTransform[1][1], nodeTransform[1][2], nodeTransform[1][3],
                        nodeTransform[2][0], nodeTransform[2][1], nodeTransform[2][2], nodeTransform[2][3],
                        nodeTransform[3][0], nodeTransform[3][1], nodeTransform[3][2], nodeTransform[3][3]
                    };
                }
                else
                {
                    std::cerr << "WARNING: unexpected node index returned by Bg2ModelExport.addBg2Model(): " << nodeIndex << std::endl;
                }
            }
        }
    }
    if (node["children"]->isList())
    {
        for (const auto& child : node["children"]->listValue())
        {
            if (child->isObject())
            {
                parseNode(child->objectValue(), transform, basePath, outPath);
            }
        }
    }
}
