#ifndef gltf_export_bg2modelexport_hpp
#define gltf_export_bg2modelexport_hpp

#include <Bg2FileReader.hpp>

#include <tiny_gltf.h>
#include <vector>
#include <array>
#include <string>

class Bg2ModelExport {
public:
	Bg2ModelExport(tinygltf::Model&, const std::string& imageInputPath, const std::string& imageOutputPath);

	/**
	 *
	 * Adds a mesh to a model from positions, normals and texture coordinates
	 * 
	 * @param positions Positions of each vertex. It is defined in groups of three coordinates, so the length must be a multiple of three.
	 * @param normals Normals of each vertex. It's defined in groups of three coordinates for each vector, so the lenght must be the same as positions
	 * @param texCoord0 First set of texture coordinates, defined in groups of two coordinates. One coordinate must be defined for each vertex, so that: positions.size() / 3 == texCoord0 / 2
	 * @return the index of the new mesh added to the model
	 *
	 */
	int addMesh(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& texCoord0,
		const std::vector<uint32_t>& indices,
		int materialIndex
	);

	/**
	 *
	 * Adds all polyLists of a bg2 model to the gltf model
	 * 
	 * @param bg2Reader the source bg2 file reader
	 * @return the index of the last mesh added to the model
	 * 
	 */
	int addBg2Model(Bg2FileReader& bg2Reader);

private:
	std::string _imageInputPath;
	std::string _imageOutputPath;
	tinygltf::Model* _model;
	int _meshIndex = -1;
	tinygltf::Mesh _mesh;
	std::map<std::string, int> _images;
	std::map<std::string, int> _textures;


	void writeBuffer(float value, std::vector<uint8_t>& buffer);
	void writeBuffer(uint32_t value, std::vector<uint8_t>& buffer);
	void writeBuffer(uint16_t value, std::vector<uint8_t>& buffer);
	void updateRange(float x, float y, float z, std::array<float, 3>& max, std::array<float, 3>& min);
	void updateRange(float x, float y, std::array<float, 2>& max, std::array<float, 2>& min);

	int getTexture(const std::string&);
	int getImageIndex(const std::string&);
	int getSampler();

	tinygltf::Value getTextureTransform(float offsetX, float offsetY, float scaleX, float scaleY);
};

#endif
