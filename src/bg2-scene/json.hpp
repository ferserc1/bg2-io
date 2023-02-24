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

            JsonObject _objectValue = {};
            JsonList _listValue = {};
            std::string _stringValue = "";
            float _numberValue = 0.0f;
            bool _boolValue = false;

            Type type;

        public:
            JsonNode();
            JsonNode(Type t);
            JsonNode(const JsonObject&);
            JsonNode(JsonObject&&);
            JsonNode(const JsonList&);
            JsonNode(JsonList&&);
            JsonNode(const char*);
            JsonNode(std::string&&);
            JsonNode(const std::string &);
            JsonNode(char);
            JsonNode(int32_t);
            JsonNode(float);
            JsonNode(double);
            JsonNode(bool);
            virtual ~JsonNode();
            
            JsonObject& objectValue() {
                if (type == Type::Object) {
                    return _objectValue;
                }
                throw std::logic_error("Improper return type: object");
            }

            const JsonObject& objectValue() const {
                if (type == Type::Object) {
                    return _objectValue;
                }
                throw std::logic_error("Improper return type: object");
            }

            JsonList& listValue() {
                if (type == Type::List) {
                    return _listValue;
                }
                throw std::logic_error("Improper return type: object");
            }

            const JsonList& listValue() const {
                if (type == Type::List) {
                    return _listValue;
                }
                throw std::logic_error("Improper return type: object");
            }

            const std::string& stringValue() {
                if (type == Type::String) {
                    return _stringValue;
                }
                throw std::logic_error("Improper return type: string");
            }

            float numberValue() {
                if (type == Type::Number) {
                    return _numberValue;
                }
                throw std::logic_error("Improper return type: number");
            }

            bool boolValue() {
                if (type == Type::Bool) {
                    return _boolValue;
                }
                throw std::logic_error("Improper return type: boolean");
            }

            void setValue(const JsonObject& object) {
                _objectValue = object;
                type = Type::Object;
            }
            
            void setValue(JsonObject&& object) {
                _objectValue = std::move(object);
                type = Type::Object;
            }

            void setValue(const JsonList& list) {
                _listValue = list;
                type = Type::List;
            }
            
            void setValue(JsonList&& list) {
                _listValue = std::move(list);
                type = Type::List;
            }

            void setValue(const char* str) {
                _stringValue = std::string(str);
                type = Type::String;
            }
            
            void setValue(const std::string& str) {
                _stringValue = std::move(str);
                type = Type::String;
            }
            
            void setValue(std::string&& str) {
                _stringValue = std::move(str);
                type = Type::String;
            }
            
            void setValue(char str) {
                _stringValue = std::to_string(str);
                type = Type::String;
            }
            
            void setValue(int32_t n) {
                _numberValue = static_cast<float>(n);
                type = Type::Number;
            }
            void setValue(float n) {
                _numberValue = n;
                type = Type::Number;
            }

            void setValue(double n) {
                _numberValue = static_cast<float>(n);
                type = Type::Number;
            }

            void setValue(bool b) {
                _boolValue = b;
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

            void printNode(int indentationLevel = 0);

            std::string toString(int indentationLevel = 0);
        };
    }

}

#endif
