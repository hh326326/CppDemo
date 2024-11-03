#include "Query.h"
#include "TextQuery.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

TextQuery get_file(int argc, char **argv) {
    ifstream infile;
    if (argc == 2)
        infile.open(argv[1]);
    if(!infile) 
        cerr << "No input file!" << endl;

    return TextQuery(infile);
}

bool get_word(string &s1) {
    cout << "enter a word to search for, or q to quit: ";
    cin >> s1;
    if(!cin || s1 == "q") return false;
    else return  true;
}

bool get_words(string &s1, string &s2)
{
    cout << "enter two words to search for, or q to quit: ";
    cin >> s1;

    if(!cin || s1 == "q")
        return false;
    cin >> s2;
    return true;
}

