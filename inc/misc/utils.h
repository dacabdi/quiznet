#ifndef __UTILS__H__
#define __UTILS__H__

#include <string>
#include <vector>
#include <map>

namespace utils
{
    std::vector<std::string> split(
        const std::string& str,
        const std::string& delimiter = ",");

    // templated functions cannot be defined in object files
    template<typename K, typename V>
    bool mapsEqual(const std::map<K, V>& m1, const std::map<K, V>& m2)
    {
        typename std::map<K, V>::const_iterator it1 = m1.begin();
        typename std::map<K, V>::const_iterator it2 = m2.begin();

        for(; it1 != m1.end() && it2 != m2.end(); ++it1, ++it2)
            if(    (*it1).first  != (*it2).first 
                || (*it1).second != (*it2).second)
                        return false;

        // one of them hasn't gotten to the end
        if(it1 != m1.end() || it2 != m2.end())
            return false;

        return true;
    }

    template<typename K, typename V>
    std::vector<K> getAllKeys(const std::map<K,V>& m)
    {
        std::vector<K> v;
        
        for(const std::pair<K,V>& pair : m)
            v.push_back(pair.first);

        return v;
    }
}

#endif