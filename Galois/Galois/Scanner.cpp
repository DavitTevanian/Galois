#include "Scanner.h"
#include <ctype.h>

Scanner::Scanner(const std::string& source) :
    source_(source), 
    line_(1), 
    position_(0)
{
    keywords_.insert({"print", Kind::Print});
}

Token Scanner::scan()
{
    // reach end of source
    if (position_ == source_.size())
        return Token(Kind::EoS, line_);

    char ch = source_[position_++];

    // skip the leading whitespaces
    while ((ch == ' ' || ch == '\t') && inRange())
        ch = source_[position_++];

    // match comment
    if (ch == '#')
    {     
        while (ch != '\n' && inRange())
            ch = source_[position_++];
    }

    // match identifier or keyword
    if (isalpha(ch))
        return keywordOrId();

    // match number
    if (isdigit(ch))
        return number();

    // match metasymbols
    if (ch == '\n')
        return Token(Kind::NL, line_++);

    Kind kind = Kind::Unknown;
    switch (ch)
    {
    case ':=':
        kind = Kind::Define;
        break;
    case '=':
        kind = Kind::Assign;
        break;
    case '\'':
        kind = Kind::Derive;
        break;
    case '^':
        kind = Kind::Power;
        break;
    case '+':
        kind = Kind::Add;
        break;
    case '-':
        kind = Kind::Sub;
        break;
    case '*':
        kind = Kind::Mult;
        break;
    case '/':
        kind = Kind::Div;
        break;
    case '(':
        kind = Kind::LeftBrace;
        break;
    case ')':
        kind = Kind::RightBrace;
        break;
    }
    return Token(kind, line_, std::string(1, ch));

    // no matching
    return Token(Kind::Unknown, line_);
}

Token Scanner::keywordOrId()
{
    int  begin = position_ - 1;
    char ch    = source_[begin];
    while ((isalpha(ch) || isdigit(ch)) && inRange())
        ch = source_[position_++];
    if (!isalpha(ch) && !isdigit(ch))
        --position_;

    std::string lexeme = source_.substr(begin, position_ - begin);
    return keywords_.find(lexeme) == keywords_.end() ? Token(Kind::Identifier,  line_, lexeme)
                                                     : Token(keywords_[lexeme], line_, lexeme);
}

Token Scanner::number()
{
    int  begin = position_ - 1;
    char ch    = source_[begin];
    while (isdigit(ch) && inRange())
        ch = source_[position_++];
    if (!isdigit(ch))
        --position_;

    std::string lexeme = source_.substr(begin, position_ - begin);
    return Token(Kind::Number, line_, lexeme);
}