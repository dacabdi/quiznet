#ifndef __SOLVEDQUESTION__H__
#define __SOLVEDQUESTION__H__

#include "Question.h"
#include "ISolvedQuestion.h"
#include <string>

class FullQuestion : public IFullQuestion
{
    public:

        FullQuestion(){};
        FullQuestion(const std::string& s);
        FullQuestion(std::stringstream& ss);
        FullQuestion(Question question, char solution);

        ~FullQuestion(){};

        char getSolution(void) const override;
        const Question& getQuestion(void) const override;
        std::string serialize(void) const override;

        bool operator==(const FullQuestion& ref) const;
        bool operator!=(const FullQuestion& ref) const;
        friend std::ostream& operator<<(std::ostream& os, 
            const FullQuestion& ref);

    protected:

        char _solution;
        Question _question;

        void init(std::stringstream& ss);
        void init(Question question, char solution);
};

#endif // __ISOLVEDQUESTION__H__