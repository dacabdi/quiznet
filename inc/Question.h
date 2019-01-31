#ifndef __QUESTION__H__
#define __QUESTION__H__

#include "IQuestion.h"
#include "utils.h"

#include <sstream> // serialize/unserialize
#include <string>

#include <iostream> // TODO delete this

class Question : public IQuestion
{
    private:

        // data model

        uint32_t _id;
        std::vector<std::string> _tags;
        std::string _question;
        std::map<const char, Choice> _choices;
        char _solution;

        // deserialization methods

        uint32_t deserializeId(std::stringstream& ss) const;
        uint32_t deserializeId(const std::string& s) const;

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

        char deserializeSolution(std::stringstream& ss) const;
        char deserializeSolution(const std::string& s) const;

        // serialization methods

        std::string serializeId(const uint32_t id) const;
        std::string serializeTags(
            const std::vector<std::string>& tags) const;
        std::string serializeQuestion(const std::string& question) const;
        std::string serializeChoices(
            const std::map<const char, Choice>& choices) const;
        std::string serializeSolution(const char solution) const;

        // validation

        void validate(std::map<const char,Choice> choices, char solution);

        // initialization

        void init(const std::string& str);
        void init(std::stringstream& ss);
        void init(uint32_t id, const std::string& str);
        void init(int32_t id, std::stringstream& ss);
        void init(uint32_t id, 
                  std::vector<std::string> tags,
                  std::string question,
                  std::map<const char, Choice> choices,
                  char solution);


    public:

        // create from a string reading id
        Question(const std::string& str);

        // create from stream reading id
        Question(std::stringstream& ss);
        
        // create from a string
        Question(uint32_t id, const std::string& str);
        
        // create from stream
        Question(uint32_t id, std::stringstream& ss);

        // create from pieces
        Question(uint32_t id,
                 std::vector<std::string> tags,
                 std::string question,
                 std::map<const char, Choice> choices,
                 char solution);

        ~Question(){};

        // public interface
        uint32_t getId(void) const override;
        std::vector<std::string> getTags(void) const override;
        std::string getQuestion(void) const override;
        std::map<const char, Choice> getAllChoices(void) const override;
        Choice getChoiceById(char id) const override;
        char getSolution(void) const override;

        // serialize the entire question
        std::string serialize(void) const override;
};

#endif // __QUESTION__H__