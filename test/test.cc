#include "tinyxml2.h"
#include <iostream>
#include <string>
#include <regex>

using namespace std;
using namespace tinyxml2;

void test0() {
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    
    if(doc.ErrorID()) {
        cerr << "LoadFile error" << endl;
        return;
    }

    XMLElement * itemNode = doc.FirstChildElement("rss")
        ->FirstChildElement("channel")
        ->FirstChildElement("item");
    /* if(itemNode) { */
    while(itemNode) {
        string title = itemNode->FirstChildElement("title")->GetText();
        string link = itemNode->FirstChildElement("link")->GetText();
        string description = itemNode->FirstChildElement("description")->GetText();
        string content = itemNode->FirstChildElement("content:encoded")->GetText();
        /* cout << "title = " << title << endl; */
        /* cout << "link = " << link << endl; */
        /* cout << "description = " << description << endl; */
        /* cout << "content = " << content << endl; */
        
        regex rg("<[^>]*>");
        description = regex_replace(description, rg, "");
        content = regex_replace(content, rg, "");
        cout << "====================" << endl;
        cout << "description = " << description << endl;
        cout << "content = " << content << endl;
    
        itemNode = itemNode->NextSiblingElement("item");
    }
}

int main()
{
    test0();
    return 0;
}

