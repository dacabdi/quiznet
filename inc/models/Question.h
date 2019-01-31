#ifndef __QUESTION__H__
#define __QUESTION__H__

#include "IQuestion.h"
#include "utils.h"

#include <sstream> // serialize/unserialize
#include <string>

class Question : public IQuestion
{
    protected:

        // data model

        std::vector<std::string> _tags;
        std::string _question;
        std::map<const char, Choice> _choices;

        // deserialization methods

        std::vector<std::string> deserializeTag(
            std::stringstream& s) const;
        std::vector<std::string> deserializeTag(
            const std::string& s) const;

        std::string deserializeQuestion(std::stringstream& ss) const;
        std::string deserializeQuestion(const std::string& s) const;

        std::map<const char, Choice> deserializeChoices(
            std::stringstream& ss) const;
        std::map<const char, Choice> deserializeChoices(
            const std::string& s) const;

        // serialization methods

        std::string serializeTags(
            const std::vector<std::string>& tags) const;
        std::string serializeQuestion(const std::string& question) const;
        std::string serializeChoices(
            const std::map<const char, Choice>& choices) const;

        // validation

        void validate(std::map<const char,Choice> choices);

        // initialization

        void init(const std::string& str);
        void init(std::stringstream& ss);
        void init(std::vector<std::string> tags,
                  std::string question,
                  std::map<const char, Choice> choices);


    public:
    
        // create from a string reading id
        Question(const std::string& str);

        // create from stream reading id
        Question(std::stringstream& ss);
        
        // create from pieces
        Question(std::vector<std::string> tags,
                 std::string question,
                 std::map<const char, Choice> choices);

        ~Question(){};

        // public interface
        std::vector<std::string> getTags(void) const override;
        std::string getQuestion(void) const override;
        std::map<const char, Choice> getAllChoices(void) const override;
        Choice getChoiceById(char id) const override;

        // serialize the entire question
        std::string serialize(void) const override;

        friend std::ostream& operator<<(
            std::ostream &os, const Question& question);
        virtual bool operator==(const Question& ref) const = 0;
        virtual bool operator!=(const Question& ref) const = 0;
};

#endif // __QUESTION__H__