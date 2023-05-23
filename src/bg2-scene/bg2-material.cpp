
#include <bg2-material.hpp>

namespace bg2scene {

Bg2Material::Bg2Material()
{
    
}

Bg2Material::Bg2Material(json::JsonObject& matData)
{
    name = matData["name"]->stringValue(name);
    matClass = matData["class"]->stringValue(matClass);
    groupName = matData["groupName"]->stringValue(groupName);
    
    if (matData["diffuse"]->isString()) {
        diffuseTexture = matData["diffuse"]->stringValue();
        diffuseColor = {1.f, 1.f, 1.f, 1.f};
    }
    else if (matData["diffuse"]->isVec4()) {
        diffuseTexture = "";
        diffuseColor = matData["diffuse"]->vec4Value();
    }
    
    if (matData["metallic"]->isString()) {
        metallicTexture = matData["metallic"]->stringValue();
        metallic = 1.0f;
    }
    else {
        metallicTexture = "";
        metallic = matData["metallic"]->numberValue(metallic);
    }
    
    if (matData["roughness"]->isString()) {
        roughnessTexture = matData["roughness"]->stringValue();
        roughness = 1.0f;
    }
    else {
        roughnessTexture = "";
        roughness = matData["roughness"]->numberValue(roughness);
    }
    
    normalTexture = matData["normal"]->stringValue("");
    ambientOcclussionTexture = matData["ambientOcclussion"]->stringValue("");
    

    metallicChannel = matData["metallicChannel"]->numberValue(metallicChannel);
    roughnessChannel = matData["roughnessChannel"]->numberValue(roughnessChannel);
    lightEmissionChannel = matData["lightEmissionChannel"]->numberValue(lightEmissionChannel);
    heightChannel = matData["heightChannel"]->numberValue(heightChannel);
    diffuseUV = matData["diffuseUV"]->numberValue(diffuseUV);
    metallicUV = matData["metallicUV"]->numberValue(metallicUV);
    roughnessUV = matData["roughnessUV"]->numberValue(roughnessUV);
    fresnelUV = matData["fresnelUV"]->numberValue(fresnelUV);
    ambientOcclussionUV = matData["ambientOcclussionUV"]->numberValue(ambientOcclussionUV);
    lightEmissionUV = matData["lightEmissionUV"]->numberValue(lightEmissionUV);
    heightUV = matData["heightUV"]->numberValue(heightUV);
    normalUV = matData["normalUV"]->numberValue(normalUV);
    alphaCutoff = matData["alphaCutoff"]->numberValue(alphaCutoff);
    isTransparent = matData["isTransparent"]->boolValue(isTransparent);
    castShadows = matData["castShadows"]->boolValue(castShadows);
    cullFace = matData["cullFace"]->boolValue(cullFace);
    unlit = matData["unlit"]->boolValue(unlit);
    visible = matData["visible"]->boolValue(visible);
    visibleToShadows = matData["visibleToShadows"]->boolValue(visibleToShadows);
    diffuseScale = matData["diffuseScale"]->vec2Value(diffuseScale);
    metallicScale = matData["metallicScale"]->vec2Value(metallicScale);
    roughnessScale = matData["roughnessScale"]->vec2Value(roughnessScale);
    fresnelScale = matData["fresnelScale"]->vec2Value(fresnelScale);
    lightEmissionScale = matData["lightEmissionScale"]->vec2Value(lightEmissionScale);
    heightScale = matData["heightScale"]->vec2Value(heightScale);
    normalScale = matData["normalScale"]->vec2Value(normalScale);
}

}
