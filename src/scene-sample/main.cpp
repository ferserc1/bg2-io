
#include <bg2-scene.hpp>

#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdexcept>
#include <sstream>

void openFile(const std::string & path, std::string & out) {
    std::ifstream fileStream(path);
    if (fileStream.is_open()) {
        out = std::string((std::istreambuf_iterator<char>(fileStream)),
                          (std::istreambuf_iterator<char>()));
    }
    else {
        throw new std::runtime_error("Could not open file " + path);
    }
}

int main(int argc, char ** argv)
{
    if (argc < 2) {
        std::cerr << "Usage:" << std::endl << "bg2-scene scene-name.vitsnj" << std::endl;
        return 1;
    }

    std::cout << argv[1] << std::endl;

    try {
        std::string buffer; 
        openFile(argv[1], buffer);

        std::stringstream sstream(buffer);

        std::cout << sstream.str() << std::endl;

        bg2scene::json::JsonParser parser(&sstream);
        auto rootNode = parser.parse();

        rootNode->printNode();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
