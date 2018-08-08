#include "xmltext.h"
using namespace std;


XmlText::XmlText(string s)
{
    type=TEXT;
    str=s;
}

string XmlText::getText()
{
    return str;
}
