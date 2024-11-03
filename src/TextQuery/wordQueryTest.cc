#include "Query.h"
#include "TextQuery.h"

#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

int main(int argc, char ** argv) {
    TextQuery file = get_file(argc, argv);

    do {
        string sought;
        if(!get_word(sought)) break;

        Query name(sought);
        const QueryResult results = name.eval(file);
        cout << "\nExecuting Query for: " << name << endl;

        print_result(cout, results);
    } while (true);
}
