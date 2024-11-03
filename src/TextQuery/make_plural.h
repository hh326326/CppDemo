#pragma once

#include <iostream>
#include <string>

using namespace std;

inline
string make_plural(size_t ctr, const string & word
                   , const string &ending)
{
    return (ctr > 1) ? word + ending : word;
}




