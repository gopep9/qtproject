#ifndef XMLNODE_H
#define XMLNODE_H
#include <vector>

class XmlNode
{
public:
    XmlNode();
    bool isAttr();
    bool isElement();
    bool isText();
    enum TYPE{
        ATTR,
        ELEMENT,
        TEXT,
        NONE
    };
    TYPE getType();
protected:
    TYPE type;
    //std::vector<XmlNode>vectorNode;//不知道有什么用
};

#endif // XMLNODE_H
