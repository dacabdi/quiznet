#ifndef __CHOICE__H__
#define __CHOICE__H__

#include "IChoice.h"
#include <sstream>

class Choice : public IChoice
{
    public:
        
        Choice(const char id, const std::string& text);
        Choice(const std::string& str);

        std::string getText() override;
        char getId() override;
        std::string serialize(void) override;

    private:

        const std::string text;
        const char id;
};

#endif // __CHOICE__H__