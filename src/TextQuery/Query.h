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
    typedef TextQuery::line_no line_no;
    virtual ~Query_base() {}
private:
    // eval返回能够匹配查询条件的结果
    virtual QueryResult eval(const TextQuery&) const = 0;
    virtual string rep() const = 0;
};

class Query
{
    friend Query operator~(const Query&);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);
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
    Query(shared_ptr<Query_base> query)
    : _p_base(query)
    {}

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

class NotQuery
: public Query_base
{
    friend Query operator~(const Query&);

    NotQuery(const Query &q)
    : _query(q)
    {}

    string rep() const {
        return "~(" + _query.rep() + ")";
    }

    QueryResult eval(const TextQuery&) const;
    Query _query;
};

class BinaryQuery
: public Query_base
{
protected:
    BinaryQuery(const Query &l, const Query &r, string s)
    : _lhs(l)
    , _rhs(r)
    , _opSym(s)
    {}

    string rep() const {
        return "(" + _lhs.rep() + " " + _opSym + " " + _rhs.rep() + ")";
    }

    Query _lhs, _rhs;
    string _opSym;
};

class AndQuery
: public BinaryQuery 
{
    friend Query operator&(const Query&, const Query&);
    AndQuery(const Query &left, const Query &right)
    : BinaryQuery(left, right, "&") 
	{ 

	}

    // concrete class: AndQuery inherits rep and defines the remaining pure virtual
    QueryResult eval(const TextQuery&) const;
};

class OrQuery
: public BinaryQuery 
{
    friend Query operator|(const Query&, const Query&);
    OrQuery(const Query &left, const Query &right)
	: BinaryQuery(left, right, "|") 
	{

	}

    QueryResult eval(const TextQuery&) const;
};
inline Query operator&(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new AndQuery(lhs, rhs));//隐式转换
}

inline Query operator|(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

inline Query operator~(const Query &operand)
{
    return shared_ptr<Query_base>(new NotQuery(operand));
}

ifstream& open_file(ifstream&, const string&);
TextQuery get_file(int, char**);
bool get_word(string &);
bool get_words(string &, string &);
ostream &print(ostream&, const QueryResult);


