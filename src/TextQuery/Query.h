#pragma once

#include "TextQuery.h"
#include "make_plural.h"

#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

class Query_base {
    friend class Query;
protected:
    virtual ~Query_base() {}
private:
    // eval返回能够匹配查询条件的结果
    virtual QueryResult eval(const TextQuery&) const = 0;
    virtual string rep() const = 0;
};

class Query
{
public:
    Query(const string&); // 生成查询词
                          //
    QueryResult eval(const TextQuery &t) const {
        return _p_base->eval(t); // 多态
    }

    string rep() const {
        return _p_base->rep();
    }
private:
    shared_ptr<Query_base> _p_base;
};

inline
ostream & operator<<(ostream &os, const Query & query) {
    return os << query.rep();
}

class WordQuery
: public Query_base
{
    friend class Query;

private:
    WordQuery(const string &s)
    : _query_word(s)
    {}

    QueryResult eval(const TextQuery &t)const
    {
        return t.query(_query_word);
    }

    string rep()const {
        return _query_word;
    }
    string _query_word;
};

inline
Query::Query(const string &s)
: _p_base(new WordQuery(s))
{}

ifstream& open_file(ifstream&, const string&);
TextQuery get_file(int, char**);
bool get_word(string &);
bool get_words(string &, string &);
ostream &print(ostream&, const QueryResult);


