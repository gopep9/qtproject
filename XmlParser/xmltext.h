#ifndef XMLTEXT_H
#define XMLTEXT_H
#include "xmlnode.h"
#include <string>
class XmlNode;
class XmlText:public XmlNode
{
public:
    XmlText(std::string s="");
    std::string getText();
private:
    std::string str;
};

#endif // XMLTEXT_H
