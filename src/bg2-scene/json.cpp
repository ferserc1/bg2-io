
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
            std::string indentation = std::string(indentationLevel * 2, ' ');
            std::string innerIndentation = std::string((indentationLevel + 1) * 2, ' ');
            std::string outputString = "";

            switch (type) {
            case Type::String:
                outputString += *value.stringValue;
                break;
            case Type::Number:
                outputString += std::to_string(value.numberValue);
                break;
            case Type::Bool:
                outputString += (value.boolValue ? "true" : "false");
                break;
            case Type::Null:
                outputString += "null";
                break;
            
            case Type::List: {
                outputString += "[";
                size_t index = 0;
                bool emptyList = true;
                for (auto node : (*value.listValue)) {
                    if (emptyList) {
                        outputString += "\n";
                    }
                    emptyList = false;
                    outputString += innerIndentation + node->toString(indentationLevel + 1);
                    if (index < (*value.listValue).size() - 1) {
                        outputString += ",";
                    }
                    outputString += "\n";
                    index++;
                }
                outputString += (emptyList ? "" : indentation) + "]";
                break;
            }
            case Type::Object: {
                outputString += "{";
                bool emptyObject = true;
                for (JsonObject::iterator i = (*value.objectValue).begin();
                    i != (*value.objectValue).end(); ++i) {
                    if (emptyObject) {
                        outputString += "\n";
                    }
                    emptyObject = false;
                    outputString += innerIndentation + i->first + ": ";
                    outputString += i->second->toString(indentationLevel + 1);
                    JsonObject::iterator next = i;
                    next++;
                    if (next != (*value.objectValue).end()) {
                        outputString += ",";
                    }
                    outputString += "\n";
                }
                outputString += (emptyObject ? "" : indentation) + "}";
                break;
            }
            }
            return outputString;
        }
    }
}