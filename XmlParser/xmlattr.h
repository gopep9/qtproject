#ifndef XMLATTR_H
#define XMLATTR_H
#include "xmlnode.h"
#include <string>
class XmlNode;
class XmlAttr:public XmlNode
{
public:
    XmlAttr(std::string key="",std::string value="");
    std::string getKey();
    std::string getValue();
private:
    std::string key;
    std::string value;
};

#endif // XMLATTR_H
