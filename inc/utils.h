#ifndef __UTILS__H__
#define __UTILS__H__

#include <string>
#include <vector>

namespace utils
{
    std::vector<std::string> split(
        const std::string& str,
        const std::string& delimiter = ",");
};

#endif