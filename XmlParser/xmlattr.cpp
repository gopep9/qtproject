#include "xmlattr.h"
using namespace std;

XmlAttr::XmlAttr(string k, string v):key(k),value(v)
{
    type=ATTR;
}

string XmlAttr::getKey()
{
    return key;
}

string XmlAttr::getValue()
{
    return value;
}
