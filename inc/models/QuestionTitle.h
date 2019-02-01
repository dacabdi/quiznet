#ifndef __QUESTIONTITLE__H__
#define __QUESTIONTITLE__H__

#include "IQuestionTitle.h"
#include <string>
#include <stdexcept>
#include <sstream>

class QuestionTitle : public IQuestionTitle
{
    public:
        
        QuestionTitle(const std::string& title);
        QuestionTitle(std::stringstream& ss);

        const std::string& getText(void) const override;
        std::string serialize(void) const override;

        friend std::ostream& operator<<(
            std::ostream &os, const QuestionTitle& question);
        virtual bool operator==(const QuestionTitle& ref);
        virtual bool operator!=(const QuestionTitle& ref);

        ~QuestionTitle(){};

    protected:

        std::string _title;

        QuestionTitle(){};

        std::string deserialize(const std::string& str);
        std::string deserialize(std::stringstream& str);
};

#endif // __IQUESTIONTITLE__H__