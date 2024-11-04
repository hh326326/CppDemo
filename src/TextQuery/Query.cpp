#include "Query.h"
#include "TextQuery.h"

#include <memory>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <vector>
#include <string>

using namespace std;

QueryResult
NotQuery::eval(const TextQuery& text) const
{
    QueryResult result = _query.eval(text);

    shared_ptr<set<line_no> > ret_lines(new set<line_no>);

	QueryResult::line_it beg = result.begin(), end = result.end();

	vector<string>::size_type sz = result.get_file()->size();
    for (size_t n = 0; n != sz; ++n) {
		if (beg == end || *beg != n) 
			ret_lines->insert(n);  // if not in result, add this line 
		else if (beg != end) 
			++beg; 
	}
	return QueryResult(rep(), ret_lines, result.get_file());
}

QueryResult
AndQuery::eval(const TextQuery& text) const
{
    QueryResult left = _lhs.eval(text), right = _rhs.eval(text);

    shared_ptr<set<line_no> > ret_lines(new set<line_no>);  

    set_intersection(left.begin(), left.end(), 
                   right.begin(), right.end(),
                   inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}

// returns the union of its operands' result sets
QueryResult
OrQuery::eval(const TextQuery& text) const
{
    QueryResult right = _rhs.eval(text), left = _lhs.eval(text);  

	shared_ptr<set<line_no> > 
		ret_lines(new set<line_no>(left.begin(), left.end()));

	ret_lines->insert(right.begin(), right.end());
    return QueryResult(rep(), ret_lines, left.get_file());
}

