#include "TextQuery.h"
#include "make_plural.h"

#include <cstring>
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <fstream>
using namespace std;

typedef map<string, shared_ptr<set<TextQuery::line_no> > > wmType;
typedef wmType::const_iterator wmIter;
typedef shared_ptr<set<TextQuery::line_no> > lineType;
typedef set<TextQuery::line_no>::const_iterator lineIter;

TextQuery::TextQuery(ifstream &is)
: _file(new vector<string>)
{
    string text;
    while (getline(is, text)) {
        _file->push_back(text);
        int n = _file->size() - 1;
        istringstream line(text);
        string word;
        while(line >> word) {
            word = cleanup_str(word);
            lineType &lines = wm[word];
            if(!lines) {
                lines.reset(new set <line_no>);
            }
            lines->insert(n);
        }
    }
}

string TextQuery::cleanup_str(const string &word) {
    string ret;
    for (const auto ch : word) {
        if (!ispunct(ch))
            ret += tolower(ch);
    }
    return ret;
}

QueryResult
TextQuery::query(const string &sought) const
{
    static lineType nodata(new set<line_no>);

    wmIter loc = wm.find(cleanup_str(sought));

    if(loc == wm.end())
        return QueryResult(sought, nodata, _file); // not found
    else
        return QueryResult(sought, loc->second, _file);
}

ostream &print_result(ostream &os, const QueryResult &qr)
{
   os << qr._sought << " occurs " << qr._lines->size() << " "
      << make_plural(qr._lines->size(), "time", "s") << endl;

   for (lineIter num = qr._lines->begin();
            num != qr._lines->end(); ++num)
       os << "\t(line " << *num + 1 << ") "
          << *(qr._file->begin() + *num) << endl;
   return os;
}
