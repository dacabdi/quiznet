#ifndef __IQUIZBOOK__H__
#define __IQUIZBOOK__H__

#include "Question.h"
#include "FullQuestion.h"

#include <string>

class IQuizBook
{
    public:
        
        // obtain question
        virtual const FullQuestion& getQuestionById(
            uint32_t id) const = 0;
        virtual const FullQuestion& getRandomQuestion(void) const = 0;
        
        // insert a new question
        virtual uint32_t insertQuestion(const FullQuestion fQuestion) = 0;

        // delete a question
        virtual bool deleteQuestionById(const uint32_t id) = 0;
        
        // delete all questions
        virtual void clear(void) = 0;

        // save and load
        virtual bool saveToFile(const std::string& path) = 0;
        virtual bool loadFromFile(const std::string& path) = 0;

        // stringify the entire quizbook
        virtual std::string serialize() const = 0;

        // number of questions
        virtual size_t size(void) const = 0;

        virtual ~IQuizBook(){};
};


#endif // __IQUIZBOOK__H__