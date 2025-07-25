
#ifndef _BG2_SCENE_JSON_PARSER_HPP_
#define _BG2_SCENE_JSON_PARSER_HPP_

#include <iostream>
#include <sstream>
#include <memory>
#include "json.hpp"
#include "json-token.hpp"

namespace bg2scene {
    namespace json {

        class JsonParser {
            std::istream * stream;
            std::shared_ptr<JsonNode> root;
            std::unique_ptr<JsonNode> current;
            JsonTokenizer tokenizer;

        public:
            JsonParser(std::istream * stream) :tokenizer(stream) {}
            JsonParser(const std::string& buffer) :_sstream(buffer), tokenizer(&_sstream) {}
            JsonParser(const char* buffer) :_sstream(buffer), tokenizer(&_sstream) {}

            std::shared_ptr<JsonNode> & parse();

            std::shared_ptr<JsonNode> parseObject();
            std::shared_ptr<JsonNode> parseString();
            std::shared_ptr<JsonNode> parseNumber();
            std::shared_ptr<JsonNode> parseList();
            std::shared_ptr<JsonNode> parseBoolean();
            std::shared_ptr<JsonNode> parseNull();
            
            std::stringstream _sstream;
        };

    }
}

#endif
