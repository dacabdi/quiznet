#include "Question.h"

// --------------- CONSTRUCTORS ------------------

Question::Question(const std::string& str)
{
    std::stringstream ss(str);
    init(ss);
}

Question::Question(std::stringstream& ss)
{
    init(ss);
}

Question::Question(TagCollection tags, 
                   QuestionTitle questionTitle, 
                   ChoiceCollection choices)
{
    init(tags, questionTitle, choices);
}

// ------------ PUBLIC INTERFACE -----------

const TagCollection& Question::getTags(void) const 
{
    return _tags;
}

const QuestionTitle& Question::getQuestionTitle(void) const
{
    return _questionTitle;
}

const ChoiceCollection& Question::getChoices(void) const
{
    return _choices;
}

std::string Question::serialize(void) const
{
    std::stringstream ss;
    ss << _tags << _questionTitle << _choices << std::flush;
    return ss.str();
}

// ------------- OPERATORS --------------------

std::ostream& operator<<(std::ostream &os, const Question& ref)
{
    os << ref.serialize();
    return os;
}

bool Question::operator==(const Question& ref) const
{
    return (
           _tags == ref._tags
        && _questionTitle == ref._questionTitle
        && _choices == ref._choices
    );
}

bool Question::operator!=(const Question& ref) const
{
    return !operator==(ref);
}

// ------------- PRIVATE METHODS --------------

void Question::init(std::stringstream& ss)
{
    TagCollection tags(ss);
    QuestionTitle questionTitle(ss);
    ChoiceCollection choices(ss);

    init(tags, questionTitle, choices);
}

void Question::init(TagCollection tags,
                    QuestionTitle questionTitle,
                    ChoiceCollection choices)
{
    _tags = tags;
    _questionTitle = questionTitle;
    _choices = choices;
}