#ifndef __IQUIZBOOK__H__
#define __IQUIZBOOK__H__

#include "SolvedQuestion.h"

#include <string>
#include <limits>

class IQuizBook
{
    public:
        
        // obtain question
        virtual const SolvedQuestion& getQuestionById(
            uint32_t id) const = 0;
        virtual const SolvedQuestion& getRandomQuestion(void) = 0;
        
        // insert a new question
        virtual uint32_t insertQuestion(
            const SolvedQuestion fQuestion) = 0;
        virtual uint32_t insertQuestion(uint32_t id, 
                    const SolvedQuestion question) = 0;

        // delete a question
        virtual SolvedQuestion deleteQuestionById(const uint32_t id) = 0;

        // determine if has question
        virtual bool hasQuestion(const uint32_t id) const = 0;

        // delete all questions
        virtual void clear(void) = 0;

        // save and load
        virtual std::ostream& writeTo(std::ostream& os) const = 0;
        virtual std::istream& readFrom(std::istream& is) = 0;

        // stringify the entire quizbook
        virtual std::string serialize() const = 0;

        // number of questions
        virtual size_t size(void) const = 0;

        virtual ~IQuizBook(){};

    protected:

        IQuizBook(){};
};


#endif // __IQUIZBOOK__H__