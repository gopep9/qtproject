//#include "xmlparser.h"
//#include "xmlelement.h"
//#include <ctype.h>
//#include <QString>
//#include <stack>
//#include <deque>
//using namespace std;

//XmlParser::XmlParser()
//{

//}

//XmlParser::XmlParser(std::string str)
//{
//    xmlString=str;

//}

//void XmlParser::setXmlText(std::string str)
//{
//    std::string word;
////    while((word=getItem(str))!="")
////    {
////        std::string tmp=word;
////    }
////    str=string("<hello attr=\"a\">ffffff</hello><fffsd/>");
//    string strb=getChildElement(str);
//    string strf=getElement(strb);
//    string strg=getElement(strb);
////    string strc=getElement(str);
////    string strd=getTextFromElement(strb);
////    string stre=getAttrFromElement(strb,"attr");
//    return ;
//}

//XmlElement XmlParser::getFirstElement()
//{
////    return xmlElement();
//    XmlElement e;
//    return e;
//}

//void XmlParser::parserXml()
//{
////    string str=xmlString;
////    string word;
////    while((word=getWord(str))!="")
////    {

////    }

//}
////获取单词
//std::string XmlParser::getWord(std::string &str)
//{
//    if(str.empty())
//        return "";
//    std::string::iterator it=str.begin();
//    std::string word="";
//    //去除开始的空白
//    while(it!=str.end())
//    {
//        if(!isspace(*it))
//            break;
//        it++;
//    }
//    if(it==str.end())
//        return "";
//    if('>'==*it){
//        word=*it;
//        it++;
////        str.assign(it,str.end());
//        if('/'==*it)
//        {
//            word.push_back(*it);
//            it++;
//        }
//        str.assign(it,str.end());
//        return word;
//    }
//    else if('<'==*it){
//        word=*it;
//        it++;
////        str.assign(it,str.end());
//        if('/'==*it)
//        {
//            word.push_back(*it);
//            it++;
//        }
//        str.assign(it,str.end());
//        return word;
//    }
//    else if('/'==*it){
//        word=*it;
//        it++;
////        str.assign(it,str.end());
//        if('>'==*it)
//        {
//            word.push_back(*it);
//            it++;
//        }
//        str.assign(it,str.end());
//        return word;
//    }
//    else if('\"'==*it){
////        word=*it;
////        it++;
////        str.assign(it,str.end());
////        if()
//        word=*it;
//        it++;
//        while(str.end()!=it)
//        {
//            word.push_back(*it);
//            if('\"'==*it)
//                break;
//            it++;
//        }
//        if(str.end()!=it)
//            it++;
//        str.assign(it,str.end());
//        return word;
//    }
//    else if('='==*it){
//        word=*it;
//        it++;
//        str.assign(it,str.end());
//        return word;
//    }
//    else if('?'==*it){
//        word=*it;
//        it++;
//        str.assign(it,str.end());
//        return word;
//    }
//    else {
//        word=*it;
//        it++;
//        while((str.end()!=it)&&!isspace(*it))
//        {
//            if(*it=='<'||*it=='>'||*it=='/'||*it=='\"'||*it=='='||*it=='?')
//                break;
//            word.push_back(*it);
//            it++;
//        }
//        str.assign(it,str.end());
//        return word;
//    }
//    return word;
//}

//string XmlParser::getItem(string &str)
//{
//    string item="";
//    string word=getWord(str);
//    if((word=="<")||(word=="</")){
//        item.append(word);
//        while((word!=">")&&(word!="/>")&&(word!=""))
//        {
//            word=getWord(str);
//            item.append(word);
//            item.append(" ");
//        }
//        item.pop_back();
//        return item;
//    }
//    else
//    {
//        item.append(word);
//        while(!str.empty())
//        {
//            word=str.front();
//            if(word=="<")
//                break;
//            str.erase(0,1);
//            item+=word;
//        }
//        return item;
////        while((word!="<")&&(word!="</")&&(word!=""))
////        {
////            item.append(word);
////            word=getWord(str);
////        }
////        str.insert(0,word);
////        return item;
//    }
//}

//string XmlParser::getChildElement(string element)
//{
//    getItem(element);
//    return element;
//}

//string XmlParser::getElement(string &str)
//{
////    string str=s;
//    deque<string>vecStr;
//    stack<string> stackStr(vecStr);
//    string item;
//    string element;
//    while((item=getItem(str))!="")
//    {
//        if(getItemType(item)==open)
//        {
//            stackStr.push(getItemName(item));
//        }
//        else if(getItemType(item)==close)
//        {
//            string tmp1=stackStr.top();
//            string tmp2=getItemName(item);
//            if(stackStr.top()==getItemName(item))
//            {
//                stackStr.pop();
//            }
//            else
//            {
//                errorNo=1;
//                return "";
//            }
//        }
//        element+=item;
//        if(stackStr.empty())
//            break;
//    }
//    return element;
//}

//string XmlParser::getTextFromElement(string str)
//{
//    getItem(str);
//    return getItem(str);
//}

//string XmlParser::getAttrFromElement(string element, string attr)
//{
//    string item=getItem(element);
//    getWord(item);
//    getWord(item);
//    string word;
//    while((word=getWord(item))!="")
//    {
//        if(word==attr)
//        {
//            getWord(item);
//            return getWord(item);
//        }
//    }
//    return "";
//}
////获取item的类型，以一个<>闭合作为一个item，或者是一段string作为一个item
//XmlParser::ItemType XmlParser::getItemType(string item)
//{
//    if(item.empty())
//        return none;
//    if('<'==item[0]&&'>'==item[item.length()-1])
//    {
//        if('/'==item[item.length()-2])
//            return full;
//        else if('/'==item[1])
//            return close;
//        else
//            return open;
//    }
//    return text;
//}
////获取item的名字<a /> 的话返回a
//string XmlParser::getItemName(string item)
//{
//    ItemType type=getItemType(item);
//    if(text==type||none==type)
//    {
//        return "";
//    }
//    else
//    {
//        getWord(item);
//        return getWord(item);
////        item.assign(item.begin()+1,item.end()-1);
////        return item;
//    }
//}



