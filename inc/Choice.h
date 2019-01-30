#ifndef __CHOICE__H__
#define __CHOICE__H__

#include "IChoice.h"
#include <sstream>
#include <stdexcept>

class Choice : public IChoice
{
    private:

        char id;
        std::string text;
        
        bool validate(const std::string& str);

    public:
        
        Choice(const char id, const std::string& text);
        Choice(const std::string& str);

        std::string getText() override;
        char getId() override;
        std::string serialize(void) const override;
};

#endif // __CHOICE__H__