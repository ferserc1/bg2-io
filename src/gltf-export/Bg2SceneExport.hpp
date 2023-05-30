#ifndef gltf_export_bg2sceneexport_hpp
#define gltf_export_bg2sceneexport_hpp

#include <json.hpp>

#include <tiny_gltf.h>

#include <glm/mat4x4.hpp>

class Bg2SceneExport {
public:
    Bg2SceneExport(tinygltf::Model&);

    void addSceneFile(const std::string&, const std::string& outPath);

private:
    tinygltf::Model* _model;

    void parseNode(bg2scene::json::JsonObject&, const glm::mat4 &, const std::string& basePath, const std::string& outPath);
};

#endif
