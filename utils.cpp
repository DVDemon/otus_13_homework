#include "utils.h"

bool is_integer(const std::string& s)
{
    return !s.empty() && 
           (std::count_if(s.begin(), s.end(), [](auto a){return (a>='0'&&a<='9');}) == (long)s.size());
}

std::string replace(const std::string &input, const std::string &search, const std::string &replace)
{
    std::string result = input;
    for (size_t pos = 0;; pos += replace.length())
    {
        // Locate the substring to replace
        pos = input.find(search, pos);
        if (pos == std::string::npos)
            break;
        // Replace by erasing and inserting
        result.erase(pos, search.length());
        result.insert(pos, replace);
    }
    return result;
}