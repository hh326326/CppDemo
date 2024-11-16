#pragma once

#include "tinyxml2.h"                                                  
#include <iostream>
#include <string>
#include <regex>
#include <fstream>

using namespace std;
using namespace tinyxml2;

struct RssItem
{
    string _title;
    string _link;
    string _description;
    string _content;
};

class RssReader
{
public:
    RssReader(size_t capa);
    void parseRss(const string & filename); // 解析
    void store(const string & filename);//输出
private:
    vector<RssItem> _rss;
 };   

