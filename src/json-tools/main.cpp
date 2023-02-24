
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include <json.hpp>
#include <json-parser.hpp>

void createJsonFile(const std::string& filePath)
{
    using namespace bg2scene::json;
    
    auto rootNode = std::make_shared<JsonNode>();
    rootNode->setValue(JsonObject{
        {"node1", std::make_shared<JsonNode>(std::string("Test"))},
        {"node2", std::make_shared<JsonNode>(17.59)},
        {"node3", std::make_shared<JsonNode>(true)},
        {"node4", std::make_shared<JsonNode>(42)},
        {"node5", std::make_shared<JsonNode>(JsonObject{
            {
                "child1", std::make_shared<JsonNode>(JsonList{
                    std::make_shared<JsonNode>(1),
                    std::make_shared<JsonNode>(2),
                    std::make_shared<JsonNode>(3),
                    std::make_shared<JsonNode>(4),
                    std::make_shared<JsonNode>(5)
                })
            },
            {
                "child2", std::make_shared<JsonNode>(JsonList{
                    std::make_shared<JsonNode>("Apple"),
                    std::make_shared<JsonNode>("Banana"),
                    std::make_shared<JsonNode>("Pear"),
                    std::make_shared<JsonNode>("Strawberry")
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
