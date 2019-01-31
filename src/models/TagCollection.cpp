#include "TagCollection.h"

// ----------- CONSTRUCTORS ------------

TagCollection::TagCollection(const std::string& str)
{
    _tags = deserialize(str);
}

TagCollection::TagCollection(std::stringstream& ss)
{
    std::string str;
    std::getline(ss, str);
    _tags = deserialize(str);
}

// ----------- PUBLIC INTERFACE ------------

inline const Tag& TagCollection::at(const size_t idx) const
{
    return _tags.at(idx);
}

const std::vector<Tag>& TagCollection::getAllTags(void) const
{
    return _tags;
}

size_t TagCollection::size(void) const
{
    return _tags.size();
}


std::string TagCollection::serialize(void) const
{
    std::stringstream ss;
    std::vector<Tag>::const_iterator it = _tags.begin();
    
    // consider no tags
    if (it != _tags.end())
    {
        // take in the first one without preceding comma
        ss << (*it).serialize();
        ++it;
        
        // do the remaining with preceding commas
        for(; it != _tags.end(); ++it)
        {
            // TODO should strip spaces? and then fix this
            ss << "," << (*it).serialize();
        }
    }
    
    return ss.str();
}

// ----------- OPERATORS ------------

bool TagCollection::operator==(const TagCollection& ref) const
{
    const std::vector<Tag> v1 = getAllTags();
    const std::vector<Tag> v2 = ref.getAllTags();

    std::vector<Tag>::const_iterator it1 = v1.begin();
    std::vector<Tag>::const_iterator it2 = v2.begin();

    for(; it1 != v1.end() && it2 != v2.end(); ++it1, ++it2)
        if(*it1 != *it2) 
            return false;

    // one of them hasn't gotten to the end
    if(it1 != v1.end() || it2 != v2.end())
        return false;

    return true;
}

bool TagCollection::operator!=(const TagCollection& ref) const
{
    return !operator==(ref);
}

std::ostream& operator<<(std::ostream &os, const TagCollection& ref)
{
    os << ref.serialize();
    return os;
}

/*const Tag& TagCollection::operator[](const size_t idx) const
{
    return at(idx);
}*/


// ----------- PRIVATE METHODS ------------

std::vector<Tag> TagCollection::deserialize(const std::string& str)
{
    std::string s = str;
    std::vector<Tag> output;
    std::string token;
    size_t pos = 0; 

    while ((pos = s.find(',')) != std::string::npos) {
        token = s.substr(0, pos);
        output.push_back(Tag(token));
        s.erase(0, pos + 1);
    }

    output.push_back(s);

    return output;
}



