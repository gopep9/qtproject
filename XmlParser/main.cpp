#include <QCoreApplication>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QDomAttr>
#include <QDomText>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
//#include <xmlparser.h>
#include "xmlelement.h"
using namespace std;

int main(int argc, char *argv[])
{
    int fd=open("AndroidManifest.xml",O_RDONLY|O_CLOEXEC);
    char buf[512]{};
    int readnum=0;
    std::string xmlstr;
    while((readnum=read(fd,buf,sizeof(buf)-1))>0)
    {
        xmlstr+=buf;
    }
    XmlElement root(xmlstr);
    puts("done");
    return 0;

}
