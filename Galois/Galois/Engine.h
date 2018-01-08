#pragma once
#include <exception>
#include <vector>

class SyntaxError : public std::exception
{
public:
    SyntaxError(const std::string& message, int line) : message_(message), line_(line) {}
    const char* what() const { return (message_ + std::to_string(line_)).c_str(); }

private:
    std::string message_;
    int         line_;
};

class SemanticError : public std::exception
{
public:
    SemanticError(const std::string& message) : message_(message) {}
    const char* what() const { return message_.c_str(); }

private:
    std::string message_;
};

class Element
{
public:
    Element() : variable_(""), coeff_(1), degree_(1), sign_('\0') {}
    Element(const std::string& variable, int coeff = 1, int degree = 1)
        : variable_(variable),
          coeff_(coeff),
          degree_(degree)
    {}
    void         sign(char sign)  { sign_ = sign;     }
    int          degree()   const { return degree_;   }
    std::string  variable() const { return variable_; }

private:
    std::string variable_;
    int         coeff_;
    int         degree_;
    char        sign_;
};

typedef std::vector<Element> Elements;

class Polynom
{
public:
    Polynom() {}
    void addElement(const Element& elem) throw(SemanticError)
    {
        Elements::const_iterator        it    = polynom_.begin();
        const  Elements::const_iterator itEnd = polynom_.end();
        for (; it != itEnd; ++it)
        {
            if (it->variable() != elem.variable()) 
                throw SemanticError("Monomial has different variable name \"" + elem.variable() + "\".");
        }
        polynom_.emplace_back(elem);
    }
    const Element& element() const { return polynom_[0]; }
private:   
    Elements polynom_;
};

class Expression
{
public:
    virtual Polynom evaluate() {}
};

class Name : public Expression
{
public:
    Name() : name_(""), variable_("") {}
    Name(const std::string& name, const std::string& variable) : name_(name), variable_(variable) {}
    void addName(const std::string& name, const std::string& variable)
    {
        name_ = name;
        variable_ = variable;
    }
    bool         isEmpty() { return name_.empty(); }
    std::string  name()     const { return name_; }
    std::string  variable() const { return variable_; }
    Polynom evaluate() override {}

private:
    std::string name_;
    std::string variable_;
};

class Definition
{
public:
    Definition() : polynom_(Poly()) {}
    Definition(const Name& name, const Polynom& polynom) : polynom_(Poly(name, polynom)) {}
    struct Poly
    {
        Poly() : name_(Name()), polynom_(Polynom()) {}
        Poly(const Name& name, const Polynom& polynom) : name_(name), polynom_(polynom) {}
        Name    name_;
        Polynom polynom_;
    };
    void addPolynom(const Name& name, const Polynom& polynom) throw(SemanticError)
    {
        if (name.variable() != polynom.element().variable())
            throw SemanticError("Variable \"" + name.variable() + "\" in polynom name and variable \"" + polynom.element().variable() + "\" in polynom expression are different.");
        polynom_ = Poly(name, polynom);
    }
    Poly polynom() const { return polynom_; }

private:
    Poly polynom_;
};

Polynom operator+(const Polynom& poly1, const Polynom& poly2)
{

}
Polynom operator-(const Polynom& poly1, const Polynom& poly2)
{

}
Polynom operator*(const Polynom& poly1, const Polynom& poly2)
{

}
Polynom operator/(const Polynom& poly1, const Polynom& poly2)
{

}

class Binary : public Expression
{
public:
    Binary(char op, Expression& expr1, Expression& expr2) : expr1_(expr1), expr2_(expr2), op_(op) {}
    virtual Polynom evaluate()
    {
        Polynom poly1 = expr1_.evaluate();
        Polynom poly2 = expr2_.evaluate();
        Polynom result;

        switch (op_)
        {
        case '+' :
            result = poly1 + poly2;
            break;
        case '-':
            result = poly1 - poly2;
            break;
        case '*':
            result = poly1 * poly2;
            break;
        case '/':
            result = poly1 / poly2;
            break;
        }
    }

private:
    Expression expr1_;
    Expression expr2_;
    char       op_;
};

class Derive : public Expression
{
public:
    Derive(const std::string name, const std::string variable, int order)
        : name_(name),
        variable_(variable),
        order_(order)
    {}
    std::string name() const { return name_; }
    Polynom evaluate() override {}

private:
    std::string name_;
    std::string variable_;
    int         order_;
};

class Eval : public Expression
{
public:
    Eval(const std::string name, int value)
        : name_(name),
        value_(value)
    {}
    std::string name() const { return name_; }
    Polynom evaluate() override {}

private:
    std::string name_;
    int         value_;
};


class Statement
{
public:
    Statement() : expr_(Expression()), name_(Name()) {}
    Statement(const Expression& expr, const Name& name) : expr_(expr), name_(name) {}
    Name        name() const { return name_; }
    Expression& expression() { return expr_; }
private:
    Expression expr_;
    Name       name_;
};

typedef std::vector<Definition> Definitions;
typedef std::vector<Statement>  Statements;

class Engine
{
public:
    void addDefinition(const Definition& definition) throw(SemanticError)
    {
        Definitions::const_iterator        it    = definitions_.begin();
        const  Definitions::const_iterator itEnd = definitions_.end();
        for (; it != itEnd; ++it)
        {
            if (it->polynom().name_.name() == definition.polynom().name_.name())
                throw SemanticError("Polynom with name \"" + definition.polynom().name_.name() + "\" already exists.");
        }
        definitions_.emplace_back(definition); 
    }
    void addStatement(const Statement& statement) throw(SemanticError)
    {
        Definitions::const_iterator        it    = definitions_.begin();
        const  Definitions::const_iterator itEnd = definitions_.end();
        for (; it != itEnd; ++it)
        {
            if (it->polynom().name_.name() == statement.name().name())
                break;
        }
        if (it != itEnd)
            throw SemanticError("Polynom with name \"" + statement.name().name() + "\" already exists.");

        statements_.emplace_back(statement);
    }
    void run()
    {
        Statements::iterator              it    = statements_.begin();
        const  Statements::const_iterator itEnd = statements_.end();
        for (; it != itEnd; ++it)
        {
            it->expression().evaluate();
        }
    }
private:
    Definitions definitions_;
    Statements  statements_;
};