#ifndef gltf_export_bg2file_hpp
#define gltf_export_bg2file_hpp

#include <string>

#include <bg2-io.h>

class Bg2FileReader {
public:
    Bg2FileReader();
    ~Bg2FileReader();

    void open(const std::string& file);
    void close();

private:
    Bg2File* _bg2File = nullptr;
};

#endif
