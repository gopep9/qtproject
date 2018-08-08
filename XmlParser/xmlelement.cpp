#include "xmlelement.h"
#include "xmlnode.h"
#include "xmltext.h"
#include "xmlattr.h"
#include <deque>
#include <stack>
#include <iostream>
using namespace std;

XmlElement::XmlElement(string elementStr)
{

    type=ELEMENT;
    string elementStrCopy=elementStr;
    //下面的内容elementStr被修改
    //去掉注释
    string item1=getItem(elementStr);
    while('?'==item1[1]||'!'==item1[1])//去掉开始的<?和注释
    {
        item1=getItem(elementStr);
    }

    elementStr.insert(0,item1);
    elementName=getItemName(item1);
    std::cout<<elementName<<std::endl;
    vector<string>vecstr=getChildElement(elementStr);
    for(auto str:vecstr)
    {
        if(getItemType(str)==text)
        {
            this->TextStr=XmlText(str);
            this->vecElement=std::vector<XmlElement>();
            break;
        }
        else{
            this->vecElement.push_back(XmlElement(str));
        }
    }
    auto vecPairStr=getAttrVec(elementStr);
    for(auto &pairStr:vecPairStr)
    {
        this->vecAttr.push_back(XmlAttr(pairStr.first,pairStr.second));
    }
}

string XmlElement::getElementName()
{
    return elementName;
}
//获取一个词，同时在传入的str中删除这个获取的词
string XmlElement::getWord(string&str)
{
    if(str.empty())
        return "";
    std::string::iterator it=str.begin();
    std::string word="";
//    去除开始的空白
    while(it!=str.end())
    {
        if(!isspace(*it))
            break;
        it++;
    }

    if(it==str.end())
        return "";
//    假如是>的话直接返回
    if('>'==*it){
        word=*it;
        it++;
//        str.assign(it,str.end());
        //下面的代码不知道有什么用
//        if('/'==*it)
//        {
//            word.push_back(*it);
//            it++;
//        }
        str.assign(it,str.end());
        return word;
    }
//    假如是<的话判断是</还是单是<
    else if('<'==*it){
        word=*it;
        it++;
        if('/'==*it)
        {
            word.push_back(*it);
            it++;
        }
        str.assign(it,str.end());
        return word;
    }
//    假如是/的话判断是否是/>
    else if('/'==*it){
        word=*it;
        it++;
        if('>'==*it)
        {
            word.push_back(*it);
            it++;
        }
        str.assign(it,str.end());
        return word;
    }
//    返回""中的内容
    else if('\"'==*it){
        word=*it;
        it++;
        while(str.end()!=it)
        {
            word.push_back(*it);
            if('\"'==*it)
                break;
            it++;
        }
        if(str.end()!=it)
            it++;
        str.assign(it,str.end());
        return word;
    }
//    返回等号
    else if('='==*it){
        word=*it;
        it++;
        str.assign(it,str.end());
        return word;
    }
//    返回问号
    else if('?'==*it){
        word=*it;
        it++;
        str.assign(it,str.end());
        return word;
    }
//    其他情况，返回一段字符，如abcd
    else {
        word=*it;
        it++;
        while((str.end()!=it)&&!isspace(*it))
        {
            if(*it=='<'||*it=='>'||*it=='/'||*it=='\"'||*it=='='||*it=='?')
                break;
            word.push_back(*it);
            it++;
        }
        str.assign(it,str.end());
        return word;
    }
    return word;
}
//获取一整个的item，同时在str中删除，item是<>或者是字符串
string XmlElement::getItem(string &str)
{
    string item="";
    string word=getWord(str);
    if((word=="<")||(word=="</")){
        item.append(word);
        while((word!=">")&&(word!="/>")&&(word!=""))
        {
            word=getWord(str);
            item.append(word);
            //统一使用一个空格分隔元素
            item.append(" ");
        }
        //删除最后的空格
        item.pop_back();
        return item;
    }
    else
    {
        item.append(word);
        while(!str.empty())
        {
            word=str.front();
            if(word=="<")
                break;
            str.erase(0,1);
            item+=word;
        }
        return item;
    }
}
//获取item的类型，开，关，闭合，文本
XmlElement::ItemType XmlElement::getItemType(string item)
{
    if(item.empty())
        return none;
    if('<'==item[0]&&'>'==item[item.length()-1])
    {
        if('/'==item[item.length()-2])
            return full;
        else if('/'==item[1])
            return close;
        else if('?'==item[1])
            return none;
        else if('!'==item[1])
            return none;
        else
            return open;
    }
    return text;
}
//获取item的名字<a /> 的话返回a
string XmlElement::getItemName(string item)
{
//    非开，关，闭合的情况返回""
    ItemType type=getItemType(item);
    if(text==type||none==type)
    {
        return "";
    }
    else
    {
        getWord(item);
        return getWord(item);
    }
}
//获取指定的属性，可能不会用上
string XmlElement::getAttrFromElement(string element, string attr)
{
    string item=getItem(element);
    getWord(item);
    getWord(item);
    string word;
    while((word=getWord(item))!="")
    {
        if(word==attr)
        {
            getWord(item);
            return getWord(item);
        }
    }
    return "";
}
//获取element中的text
string XmlElement::getTextFromElement(string str)
{
//    getItem(str);
//    return getItem(str);
}
//从当前位置获取整一个元素，或者是一个字符串，同时删除str中对应的字符
string XmlElement::getElement(string &str)
{
    deque<string>vecStr;
    stack<string> stackStr(vecStr);
    string item;
    string element;
    while((item=getItem(str))!="")
    {
        if(getItemType(item)==open)
        {
            stackStr.push(getItemName(item));
        }
        else if(getItemType(item)==close)
        {
            if(stackStr.empty())
                break;
            string tmp1=stackStr.top();
            string tmp2=getItemName(item);
            if(stackStr.top()==getItemName(item))
            {
                stackStr.pop();
            }
            else
            {
                errorNo=1;
                return "";
            }
        }else if(getItemType(item)==none)
        {
            continue;
        }
        element+=item;
        if(stackStr.empty())
            break;
    }
    return element;
}
//获取子元素列表
std::vector<string> XmlElement::getChildElement(string str)
{
    //删除最外面的元素

    string item1=getItem(str);
//    while('?'==item1[1]||'!'==item1[1])//去掉开始的<?和注释
//    {
//        item1=getItem(str);
//    }
    string element="";
    vector<string> vecstr;
    while((element=getElement(str))!="")
    {
        vecstr.push_back(element);
    }
    return vecstr;
}

std::vector<std::pair<string, string> > XmlElement::getAttrVec(string element)
{
    string item=getItem(element);
    getWord(item);
    getWord(item);
    string word;
    vector<pair<string,string>>pairStr;
    string first,second;
    while((word=getWord(item))!="")
    {
//        if(word==attr)
//        {
//            getWord(item);
//            return getWord(item);
//        }
        if('='==word[0])
        {
            continue;
        }
        else if('\"'==word[0])
        {
            second=string(word.begin()+1,word.end()-1);
            pairStr.push_back(pair<string,string>(first,second));
        }
        else {
            first=word;
        }
    }
    return pairStr;
}

