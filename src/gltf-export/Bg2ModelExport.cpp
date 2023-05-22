
#include <Bg2ModelExport.hpp>

Bg2ModelExport::Bg2ModelExport(tinygltf::Model& model)
	:_model{&model}
{

}

int Bg2ModelExport::addMesh(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& texCoord0,
	const std::vector<uint32_t>& indices
)
{
	return 0;
}


void Bg2ModelExport::writeBuffer(float value, std::vector<uint8_t>& buffer)
{
    union {
        float number;
        uint8_t bytes[4];
    } bufferData;
    bufferData.number = value;
    buffer.push_back(bufferData.bytes[0]);
    buffer.push_back(bufferData.bytes[1]);
    buffer.push_back(bufferData.bytes[2]);
    buffer.push_back(bufferData.bytes[3]);
}

void Bg2ModelExport::writeBuffer(uint32_t value, std::vector<uint8_t>& buffer)
{
    union {
        uint32_t value;
        uint8_t bytes[4];
    } bufferData;
    bufferData.value = value;
    buffer.push_back(bufferData.bytes[0]);
    buffer.push_back(bufferData.bytes[1]);
    buffer.push_back(bufferData.bytes[2]);
    buffer.push_back(bufferData.bytes[3]);
}

void Bg2ModelExport::writeBuffer(uint16_t value, std::vector<uint8_t>& buffer)
{
    union {
        uint16_t value;
        uint8_t bytes[2];
    } bufferData;
    bufferData.value = value;
    buffer.push_back(bufferData.bytes[0]);
    buffer.push_back(bufferData.bytes[1]);
}

void Bg2ModelExport::updateRange(float x, float y, float z, std::array<float, 3>& max, std::array<float, 3>& min)
{
   
}

void Bg2ModelExport::updateRange(float x, float y, std::array<float, 2>& max, std::array<float, 2>& min)
{

}
