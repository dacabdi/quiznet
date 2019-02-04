#ifndef __QUIZBOOK__H__
#define __QUIZBOOK__H__

#include "IQuizBook.h"
#include "UniformRandom.h" // getRandomQuestion
#include "utils.h" // getAllKeys

#include <stdexcept>
#include <functional>
#include <istream>

class QuizBook : public IQuizBook
{
    public:

        QuizBook(void){};
        QuizBook(const std::string& str);
        QuizBook(std::istream& is);
        ~QuizBook(){};

        // public event handlers (with do nothing stubs)
        std::function<void(const SolvedQuestion&,QuizBook*)> onInsert = 
        [](const SolvedQuestion& solvedQuestion, QuizBook* context){};

        std::function<void(SolvedQuestion,QuizBook*)> onDelete =
        [](SolvedQuestion solvedQuestion, QuizBook* context){};

        std::function<void(QuizBook*)> onClear = [](QuizBook* context){};

        // obtain question
        const SolvedQuestion& getQuestionById(uint32_t id) const override;
        const SolvedQuestion& getRandomQuestion(void) override;
        
        // insert a new question
        uint32_t insertQuestion(const SolvedQuestion fQuestion) override;
        uint32_t insertQuestion(uint32_t id, 
            const SolvedQuestion question) override;

        // delete a question
        SolvedQuestion deleteQuestionById(const uint32_t id) override;

        // check if question exists
        bool hasQuestion(const uint32_t id) const override;

        // delete all questions
        void clear(void) override;

        // save and load
        std::ostream& writeTo(std::ostream& os) const override;
        std::istream& readFrom(std::istream& is) override;

        // stringify the entire quizbook
        std::string serialize() const override;

        // number of questions
        size_t size(void) const override;

    protected:

        std::map<const uint32_t, const SolvedQuestion> _questions;

        void init(const std::string& str);
        void init(std::istream& is);

        uint32_t findFreeId(const uint32_t start = 0) const;
        
};

#endif