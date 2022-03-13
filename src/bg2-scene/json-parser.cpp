
#include "json-parser.hpp"

namespace bg2scene {
    namespace json {

            void JsonParser::parse() {
                std::string key = "";
                while (tokenizer.hasMoreTokens()) {
                    JsonToken token;
                    try {
                        token = tokenizer.getToken();
                        std::cout << token.toString() << std::endl;
                        switch (token.type) {
                        case JsonTokenType::CurlyOpen: {
                            std::shared_ptr<JsonNode> parsedObject = parseObject();
                            parsedObject->printNode(0);
                            if (!root) {
                                root = parsedObject;
                            }
                            break;
                        }
                        case JsonTokenType::ListOpen: {
                            std::shared_ptr<JsonNode> parsedList = parseList();
                            if (!root) {
                                root = parsedList;
                            }
                            break;
                        }
                        case JsonTokenType::String: {
                            tokenizer.rollBackToken();
                            std::shared_ptr<JsonNode> parsedString = parseString();
                            if (!root) {
                                root = parsedString;
                            }
                            break;
                        }
                        case JsonTokenType::Number: {
                            tokenizer.rollBackToken();
                            std::shared_ptr<JsonNode> parsedNumber = parseNumber();
                            if (!root) {
                                root = parsedNumber;
                            }
                        }
                        case JsonTokenType::Boolean: {
                            tokenizer.rollBackToken();
                            std::shared_ptr<JsonNode> parsedBoolean = parseBoolean();
                            break;
                        }
                        default:
                            throw std::logic_error("Unexpected token");
                        }
                    }
                    catch(std::logic_error &e) {
                        throw e;
                    }
                }
            }

            std::shared_ptr<JsonNode> JsonParser::parseObject() {
                std::cout << "Parsing object " << std::endl;
                std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
                JsonObject * keyObjectMap = new JsonObject();
                bool hasCompleted = false;
                while (!hasCompleted) {
                    if (tokenizer.hasMoreTokens()) {
                        JsonToken nextToken = tokenizer.getToken();
                        std::string key = nextToken.value;
                        std::cout << key << std::endl;
                        tokenizer.getToken();
                        nextToken = tokenizer.getToken();
                        std::shared_ptr<JsonNode> node;
                        switch (nextToken.type) {
                        case JsonTokenType::String:
                            tokenizer.rollBackToken();
                            (*keyObjectMap)[key] = parseString();
                            break;
                        case JsonTokenType::ListOpen:
                            (*keyObjectMap)[key] = parseList();
                            break;
                        case JsonTokenType::Number:
                            tokenizer.rollBackToken();
                            (*keyObjectMap)[key] = parseNumber();
                            break;
                        case JsonTokenType::CurlyOpen:
                            (*keyObjectMap)[key] = parseObject();
                            break;
                        case JsonTokenType::Boolean:
                            tokenizer.rollBackToken();
                            (*keyObjectMap)[key] = parseBoolean();
                            break;
                        case JsonTokenType::NullType:
                            (*keyObjectMap)[key] = parseNull();
                            break;
                        default:
                            throw std::logic_error("Unexpected token");
                        }        
                    }
                    else {
                        throw std::logic_error("No more tokens");
                    }
                }
                node->setValue(keyObjectMap);
                return node;
            }
            
            std::shared_ptr<JsonNode> JsonParser::parseString() {
                std::cout << "Parsing string" << std::endl;
                std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
                JsonToken token = tokenizer.getToken();
                std::string *sValue = new std::string(token.value);
                node->setValue(sValue);
                return node;
            }
            
            std::shared_ptr<JsonNode> JsonParser::parseNumber() {
                std::cout << "Parsing number" << std::endl;
                std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
                JsonToken nextToken = tokenizer.getToken();
                std::string value = nextToken.value;
                std::cout << value << std::endl;
                float fValue = std::stof(value);
                node->setValue(fValue);
                return node;
            }
            
            std::shared_ptr<JsonNode> JsonParser::parseList() {
                std::cout << "Parsing list" << std::endl;
                std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
                JsonList * list = new JsonList();
                bool hasCompleted = false;
                while (!hasCompleted) {
                    if (!tokenizer.hasMoreTokens()) {
                        throw std::logic_error("No more tokens");
                    }
                    else {
                        JsonToken nextToken = tokenizer.getToken();
                        std::shared_ptr<JsonNode> node;
                        switch (nextToken.type) {
                        case JsonTokenType::ListOpen:
                            node = parseList();
                            break;
                        case JsonTokenType::CurlyOpen:
                            node = parseObject();
                            break;
                        case JsonTokenType::String:
                            tokenizer.rollBackToken();
                            node = parseString();
                            break;
                        case JsonTokenType::Number:
                            tokenizer.rollBackToken();
                            node = parseNumber();
                            break;
                        case JsonTokenType::Boolean:
                            tokenizer.rollBackToken();
                            node = parseBoolean();
                            break;
                        case JsonTokenType::NullType:
                            node = parseNull();
                            break;
                        default:
                            throw std::logic_error("Unexpected token");
                        }
                        list->push_back(node);
                        nextToken = tokenizer.getToken();
                        if (nextToken.type == JsonTokenType::ListClose) {
                            hasCompleted = true;
                        }
                    }
                }
                node->setValue(list);
                return node;
            }
            
            std::shared_ptr<JsonNode> JsonParser::parseBoolean() {
                std::cout << "Parsing boolean" << std::endl;
                std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
                JsonToken nextToken = tokenizer.getToken();
                node->setValue(nextToken.value == "true" ? true : false);
                return node;
            }
            
            std::shared_ptr<JsonNode> JsonParser::parseNull() {
                std::cout << "Parsing null" << std::endl;
                std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
                node->setNull();
                return node;
            }
            
    }
}
