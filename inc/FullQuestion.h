#ifndef __FULLQUESTION__H__
#define __FULLQUESTION__H__

#include "Question.h"
#include "IFullQuestion.h"

class FullQuestion : public IFullQuestion
{
    protected:

        Question * _question;
        char _solution;

        // serialize / deserialize

        std::string serializeSolution(const char solution) const;
        char deserializeSolution(std::stringstream& ss) const;
        char deserializeSolution(const std::string& s) const;

        // validation

        void validate(const Question& question,
                      const char solution) const;
        
        // initialization

        void init(const std::string& str);
        void init(std::stringstream& ss);
        void init(std::vector<std::string> tags,
                  std::string question,
                  std::map<const char, Choice> choices,
                  const char solution);
        void init(const Question question, char solution);
        
    public:

        // create from a string
        FullQuestion(const std::string& str);
        // create from stream
        FullQuestion(std::stringstream& ss);
        // create from pieces
        FullQuestion(std::vector<std::string> tags,
                     std::string question,
                     std::map<const char, Choice> choices,
                     char solution);
        // create from an existing question by adding a solution
        FullQuestion(const Question question, char solution);

        ~FullQuestion(void);

        char getSolution(void) const;

        // gets the underlying question without solution
        const Question& getQuestion(void) const override;

        std::string serialize(void) const override;
};

#endif // __FULLQUESTION__H__