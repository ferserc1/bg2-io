
#include "json.hpp"
#include <sstream>
#include <iostream>

namespace bg2scene {
    namespace json {

        JsonNode::JsonNode() :type(Type::Null)
        {
            
        }

        JsonNode::JsonNode(Type t) :type(t)
        {

        }
        
        JsonNode::~JsonNode() {
            
        }

        void JsonNode::printNode(int indentationLevel) {
            std::cout << toString(indentationLevel);
        }

        std::string JsonNode::toString(int indentationLevel) {
            std::string spaceString = std::string(indentationLevel, ' ');
            std::string outputString = "";

            switch (type) {
            case Type::String:
                outputString += spaceString + *value.stringValue;
                break;
            case Type::Number:
                outputString += spaceString + std::to_string(value.numberValue);
                break;
            case Type::Bool:
                outputString += spaceString + (value.boolValue ? "true" : "false");
                break;
            case Type::Null:
                outputString += spaceString + "null";
                break;
            
            case Type::List: {
                outputString += "[\n";
                size_t index = 0;
                for (auto node : (*value.listValue)) {
                    outputString += node->toString(indentationLevel + 1);
                    if (index < (*value.listValue).size() - 1) {
                        outputString += spaceString + ", ";
                    }
                    index++;
                }
                outputString += spaceString + "]\n";
                break;
            }
            case Type::Object: {
                outputString += "{\n";
                for (JsonObject::iterator i = (*value.objectValue).begin();
                    i != (*value.objectValue).end(); ++i) {
                    outputString += spaceString + i->first + ": ";
                    outputString += i->second->toString(indentationLevel + 1);
                    JsonObject::iterator next = i;
                    next++;
                    if (next != (*value.objectValue).end()) {
                        outputString += spaceString + ", ";
                    }
                    outputString += spaceString + "\n";
                }
                outputString += "}\n";
                break;
            }
            }
            return outputString;
        }
    }
}