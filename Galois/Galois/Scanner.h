#pragma once
#include <map>
#include <string>

enum class Kind
{
    Print,
    Identifier,
    Number,
    Define,
    Assign,
    Power,
    Derive,
    LeftBrace,
    RightBrace,
    Add,
    Sub,
    Mult,
    Div,
    NL,
    EoS,
    Unknown
};

struct Token
{
    Token(const Kind& kind = Kind::Unknown, int line = 0, std::string lexeme = "")
        : kind_(kind),
        line_(line), 
        lexeme_(lexeme)
    {}
    bool is(const Kind& kind) const { return kind_ == kind; }

    Kind        kind_;
    int         line_;
    std::string lexeme_;
};

class Scanner
{
public:
    Scanner(const std::string& source);
    Token scan();
    int   line() const { return line_; }
private:   

    bool  inRange() const { return position_ < source_.size(); }
    Token keywordOrId();
    Token number();

    std::string                  source_;
    int                          line_;
    int                          position_;
    std::map<std::string, Kind>  keywords_;
};