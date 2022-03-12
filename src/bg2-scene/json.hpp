#ifndef _BG2_SCENE_JSON_HPP_
#define _BG2_SCENE_JSON_HPP_

#include <string>
#include <map>
#include <vector>
#include <exception>
#include <memory>

namespace bg2scene {

    namespace json {

        class JsonNode;

        using JsonObject = std::map<std::string, std::shared_ptr<JsonNode>>;

        using JsonList = std::vector<std::shared_ptr<JsonNode>>;

        class JsonNode {
            enum class Type {
                Object,
                List,
                String,
                Number,
                Bool,
                Null
            };
            
            union Value {
                JsonObject * objectValue;
                JsonList * listValue;
                std::string * stringValue;
                float numberValue;
                bool boolValue;
            } value;

            Type type;

        public:

            JsonNode();
            JsonNode(Type t);
            virtual ~JsonNode();
            
            auto objectValue() {
                if (type == Type::Object) {
                    return * value.objectValue;
                }
                throw std::logic_error("Improper return type: object");
            }

            auto listValue() {
                if (type == Type::List) {
                    return * value.listValue;
                }
                throw std::logic_error("Improper return type: object");
            }

            auto stringValue() {
                if (type == Type::String) {
                    return * value.stringValue;
                }
                throw std::logic_error("Improper return type: string");
            }

            auto numberValue() {
                if (type == Type::Number) {
                    return value.numberValue;
                }
                throw std::logic_error("Improper return type: number");
            }

            auto boolValue() {
                if (type == Type::Bool) {
                    return value.boolValue;
                }
                throw std::logic_error("Improper return type: boolean");
            }


            void setValue(JsonObject * object) {
                this->value.objectValue = object;
                type = Type::Object;
            }

            void setValue(JsonList * list) {
                this->value.listValue = list;
                type = Type::List;
            }

            void setString(std::string * str) {
                this->value.stringValue = str;
                type = Type::String;
            }

            void setNumber(float n) {
                this->value.numberValue = n;
                type = Type::Number;
            }

            void setBool(bool b) {
                this->value.boolValue = b;
                type = Type::Bool;
            }
            
            void setNull() {
                type = Type::Null;
            }

            bool isObject() {
                return type == Type::Object;
            }

            bool isList() {
                return type == Type::List;
            }

            bool isString() {
                return type == Type::String;
            }

            bool isNumber() {
                return type == Type::Number;
            }

            bool isBool() {
                return type == Type::Bool;
            }

            bool isNull() {
                return type == Type::Null;
            }

        };
    }

}

#endif
