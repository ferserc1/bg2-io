#ifndef gltf_export_bg2sceneexport_hpp
#define gltf_export_bg2sceneexport_hpp

#include <tiny_gltf.h>

class Bg2SceneExport {
public:
    Bg2SceneExport(tinygltf::Model&);

    void addSceneFile(const std::string&);

private:
    tinygltf::Model* _model;
};

#endif