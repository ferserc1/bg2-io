
#include "json.hpp"

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


    }
}