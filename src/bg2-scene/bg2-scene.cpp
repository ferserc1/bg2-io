
#include "bg2-scene.hpp"

#include <iostream>

void testScene()
{

    using namespace bg2scene;

    Node * n = new Node();
    Component * c = new Component();

    Node * n2 = new Node();
    Component * c2 = new Component();
    Component * c3 = new Component();
    n2->components.push_back(c2);
    n2->components.push_back(c3);

    n->components.push_back(c);
    n->children.push_back(n2);

    delete n;
}

