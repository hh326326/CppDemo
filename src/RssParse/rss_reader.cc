#include "rss_reader.h"

RssReader::RssReader(size_t capa)
{
    _rss.reserve(capa);
}

void RssReader::parseRss(const string & filename) {
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    if(doc.ErrorID()) {
        cerr << "loadFile fail" << endl;
        return;
    }

    size_t idx = 0;
    XMLElement * itemNode = 
        doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    while(itemNode) {
        string title = itemNode->FirstChildElement("title")->GetText();
        string link = itemNode->FirstChildElement("link")->GetText();
        string description = itemNode->FirstChildElement("description")->GetText();
        string content = itemNode->FirstChildElement("content:encoded")->GetText();

        regex reg("<[^>]*>"); // 通用去除html标签表达式
        description = regex_replace(description, reg, "");
        content = regex_replace(content, reg, "");

        RssItem rssItem;

        rssItem._title = title;
        rssItem._link = link;
        rssItem._description = description;
        rssItem._content = content;

        ++idx;
        _rss.push_back(rssItem);

        itemNode = itemNode->NextSiblingElement("item");

    }
}

void RssReader::store(const string & filename) {
    ofstream ofs(filename);
    if(!ofs) {
        cerr << "open " << filename << "fail" << endl;
        return;
    }

    for(size_t idx = 0; idx != _rss.size(); ++ idx) {
        ofs << "<doc><docid>" << idx + 1
            << "</docid><title>" << _rss[idx]._title
            << "</title><link>" << _rss[idx]._link
            << "</link><description>" << _rss[idx]._description
            << "</description><content>" << _rss[idx]._content
            << "</content></doc>";
        ofs << endl;
    }
}




