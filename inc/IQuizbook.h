#ifndef __IQUIZBOOK__H__
#define __IQUIZBOOK__H__

#include "Question.h"

#include <string>

class IQuizbook
{
    public:
        
        virtual bool saveToFile(const std::string& path) = 0;
        virtual bool loadFromFile(const std::string& path) = 0;

        virtual Question getRandomQuestion(void) = 0;
        virtual Question getQuestionById(uint32_t id) = 0;
        
        virtual uint32_t insertQuestion(Question question) = 0;

        virtual bool deleteQuestionById(uint32_t id) = 0;

        virtual std::string serialize() = 0;
};


#endif // __IQUIZBOOK__H__