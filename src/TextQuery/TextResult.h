#pragma once
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

class QueryResult
{
    friend ostream & print_result(ostream&, const QueryResult&);
public:
    typedef vector<string>::size_type line_no;
    typedef set<line_no>::const_iterator line_it;

    QueryResult(string s,
                shared_ptr<set<line_no> >p,
                shared_ptr<vector<string> >f)
    : _sought(s)
    , _lines(p)
    , _file(f)
    {}

    set<line_no>::size_type size() const {
        return _lines->size();
    }

    line_it begin()const {
        return _lines->begin();
    }

    line_it end() const {
        return _lines->end();
    }

    shared_ptr<vector<string> > get_file() {
        return _file;
    }
private:
    string _sought;
    shared_ptr<set<line_no> > _lines;
    shared_ptr<vector<string> > _file;
};
