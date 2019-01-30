#ifndef __QUESTION__H__
#define __QUESTION__H__

#include "IQuestion.h"
#include "utils.h"

#include <sstream> // serialize/unserialize

class Question : public IQuestion
{
    private:

        uint32_t id;
        std::vector<std::string> tags;
        std::string question;
        std::map<char, Choice> choices;
        char solution;

        // deserialization methods

        std::vector<std::string> deserializeTag(const std::stringstream& s);
        std::vector<std::string> deserializeTag(const std::string& s);

        std::string deserializeQuestion(const std::stringstream& ss);
        std::string deserializeQuestion(const std::string& s);

        std::map<char, Choice> deserializeChoices(const std::stringstream& ss);
        std::map<char, Choice> deserializeChoices(const std::string& s);

        char deserializeSolution(const std::stringstream& ss);
        char deserializeSolution(const std::string& s);

    public:

        // create from arguments
        Question(
            uint32_t id,
            std::vector<std::string> tags,
            std::string question,
            std::map<char, Choice> choices,
            char solution);

        // create from a string
        Question(uint32_t id, const std::string& str);
        
        // create from stream
        Question(uint32_t id, const std::istringstream& iss);

        uint32_t getId(void) override;
        std::vector<std::string> getTags(void) override;
        std::string getQuestion(void) override;
        std::map<char, Choice> getAllChoices(void) override;
        Choice getChoiceById(char id) override;
        char getSolution(void) override;

        std::string serialize(void) override;
};

#endif // __QUESTION__H__