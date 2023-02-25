
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include <json.hpp>
#include <json-parser.hpp>

void createJsonFile(const std::string& filePath)
{
    using namespace bg2scene::json;
    
    auto rootNode = JSON(JsonObject{
        {"node1", JSON("Test")},
        {"node2", JSON(17.59)},
        {"node3", JSON(true)},
        {"node4", JSON(42)},
        {"node5", JSON(JsonObject{
            {
                "child1", JSON(JsonList{
                    JSON(1),
                    JSON(2),
                    JSON(3),
                    JSON(4),
                    JSON(5)
                })
            },
            {
                "child2", std::make_shared<JsonNode>(JsonList{
                    JSON("Apple"),
                    JSON("Banana"),
                    JSON("Pear"),
                    JSON("Strawberry")
                })
            }
        })}
    });
    
    std::ofstream file;
    file.open(filePath,std::ios::out);
    
    if (file.is_open()) {
        file << rootNode->toString() << std::endl;
    }
    else {
        throw std::runtime_error("Could not open output file");
    }
}

void readJsonFile(const std::string& filePath)
{
    std::ifstream file;
    file.open(filePath, std::ios::in);
    
    if (file.is_open())
    {
        bg2scene::json::JsonParser parser(&file);
        auto rootNode = parser.parse();
        rootNode->printNode();
    }
    else
    {
        throw std::runtime_error("Could not open input JSON file");
    }
}

int main(int argc, char ** argv)
{
    
    if (argc < 2)
    {
        std::cerr << "Usage: " << std::endl << "json-tools output-file" << std::endl;
        return 1;
    }
    
    try
    {
        createJsonFile(std::string(argv[1]) + "/test.json");
        readJsonFile(std::string(argv[1]) + "/test.json");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
