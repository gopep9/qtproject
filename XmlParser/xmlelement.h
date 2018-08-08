#ifndef XMLELEMENT_H
#define XMLELEMENT_H
#include "xmlnode.h"
#include "xmlattr.h"
#include "xmltext.h"
#include <vector>
class XmlNode;
class XmlAttr;
class XmlText;
class XmlElement:public XmlNode
{
public:
    XmlElement(std::string elementStr);
    std::vector<XmlElement> getVecXmlElement();
    XmlText getXmlText();
    std::vector<XmlAttr> getVecXmlAttr();
    std::string getElementName();

private:
    std::vector<XmlAttr> vecAttr;
    XmlText TextStr;
    std::vector<XmlElement> vecElement;
    std::string elementName;
    int errorNo=0;

    //解析用函数
    std::string getWord(std::string&);
    std::string getItem(std::string&);

//    标记每个item的类型
    enum ItemType{
        open,
        close,
        full,
        text,
        none
    };
    ItemType getItemType(std::string item);
    std::string getItemName(std::string item);
    std::string getAttrFromElement(std::string element, std::string attr);
    std::string getTextFromElement(std::string str);
    std::string getElement(std::string &str);
    std::vector<std::string> getChildElement(std::string str);
    std::vector<std::pair<std::string,std::string> >getAttrVec(std::string element);
};

#endif // XMLELEMENT_H
