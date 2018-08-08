#include "xmlnode.h"
using namespace std;

XmlNode::XmlNode()
{
    type=NONE;
}

bool XmlNode::isAttr()
{
    return ATTR==type;
}

bool XmlNode::isElement()
{
    return ELEMENT==type;
}

bool XmlNode::isText()
{
    return TEXT==type;
}

XmlNode::TYPE XmlNode::getType()
{
    return type;
}
