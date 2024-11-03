#pragma once

#include "TextResult.h"
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>

using namespace std;

class QueryResult;

// TextQuery只负责查询,查询的结果交给QueryResult
class TextQuery
{
public:
    typedef vector<string>::size_type line_no;
    
    TextQuery(ifstream &);

    QueryResult query(const string&)const;
private:
    shared_ptr<vector<string>> _file; // 输出的每行内容
    map<string, shared_ptr<set<line_no> > > wm;

    static string cleanup_str(const string&);
};
