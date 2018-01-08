#include "Parser.h"
#include <iostream>

Parser::Parser(const std::string& text) : scanner_(text)
{
    lookahead_ = scanner_.scan();
}

Engine& Parser::parse() throw(SyntaxError)
{
    // Program = [NewLines] { (Definition | Statement) NewLines }.
    Engine engine;

    parseNewLines(true);

    Definition definition;
    Statement  statement;
    while (!lookahead_.is(Kind::EoS))
    {
        Parser::DSU DSU = dsu();
        if (DSU == Parser::DSU::Definition)
        {
            definition = parseDefinition();
            parseNewLines(false);
            try
            {
                engine.addDefinition(definition);
            }
            catch (const SemanticError& err)
            {
                std::cout << err.what() << std::endl;
            }
        }
        else if (DSU == Parser::DSU::Statement)
        {
            statement = parseStatement();
            parseNewLines(false);
            try
            {
                engine.addStatement(statement);
            }
            catch (const SemanticError& err)
            {
                std::cout << err.what() << std::endl;
            }     
        }
        else if (DSU == Parser::DSU::Unknown)
            throw SyntaxError("Syntax error in line: ", scanner_.line());
    }
    return engine;
}

Definition& Parser::parseDefinition() throw(SyntaxError)
{
    // Definition = Name ':=' Polynom.
    Definition def;
    Name    name    = parseName();
    match(Kind::Define);
    Polynom polynom = parsePolynom();
    try
    {
        def.addPolynom(name, polynom);
    }
    catch (const SemanticError& err)
    {
        std::cout << err.what() << std::endl;
    }
    return def;
}

Name& Parser::parseName() throw(SyntaxError)
{
    // Name = ID '(' ID ')'.
    Name name;
    std::string polyName = lookahead_.lexeme_;
    match(Kind::Identifier);
    match(Kind::LeftBrace);
    std::string variable = lookahead_.lexeme_;
    match(Kind::Identifier);
    match(Kind::RightBrace);
    name.addName(polyName, variable);
    return name;
}

Polynom& Parser::parsePolynom() throw(SyntaxError)
{
    // Polynom = ['+'|'-'] Element { ('+'|'-') Element }.
    char sign;
    if (lookahead_.is(Kind::Add) || lookahead_.is(Kind::Sub))
    {
        sign = lookahead_.lexeme_[0];
        lookahead_ = scanner_.scan();
    }

    Element elem = parseElement();
    elem.sign(sign);
    Polynom polynom;
    try
    {
        polynom.addElement(elem);
    }
    catch (const SemanticError& err)
    {
        std::cout << err.what() << std::endl;
    }
    while (lookahead_.is(Kind::Add) || lookahead_.is(Kind::Sub))
    {
        sign       = lookahead_.lexeme_[0];
        lookahead_ = scanner_.scan();
        elem       = parseElement();
        elem.sign(sign);
        try
        {
            polynom.addElement(elem);
        }
        catch (const SemanticError& err)
        {
            std::cout << err.what() << std::endl;
        }
    }
    return polynom;
}

Element& Parser::parseElement() throw(SyntaxError)
{
    // Element = NUM | [NUM] ID ['^' NUM].
    std::string variable = "";
    int         coeff    = 1;
    int         degree   = 1;

    if (lookahead_.is(Kind::Number))
    {
        coeff = std::stoi(lookahead_.lexeme_);
        lookahead_ = scanner_.scan();
    }
    if (lookahead_.is(Kind::Identifier))
    {
        variable   = lookahead_.lexeme_;
        lookahead_ = scanner_.scan();

        if (lookahead_.is(Kind::Power))
        {
            lookahead_ = scanner_.scan();
            degree     = std::stoi(lookahead_.lexeme_);
            match(Kind::Number);
        }
    }
    return Element(variable, coeff, degree);
}

Statement& Parser::parseStatement() throw(SyntaxError)
{
    // Statement = Name '=' Expr | PRINT Expr.
    Expression expr;
    Name       name;
    if (lookahead_.is(Kind::Print))
    {
        lookahead_ = scanner_.scan();
        expr       = parseExpression();
    }
    else
    {
        name = parseName();
        match(Kind::Assign);
        expr = parseExpression();
    }
    return Statement(expr, name);
}

Expression& Parser::parseExpression() throw(SyntaxError)
{
    // Expr = Mult { ( '+' | '-' ) Mult }.
    char op;
    Expression* expr1 = &parseMultiplication();
    while (lookahead_.is(Kind::Add) || lookahead_.is(Kind::Sub))
    {
        op = lookahead_.lexeme_[0];
        lookahead_ = scanner_.scan();
        Expression* expr2 = &parseMultiplication();
        expr1 = new Binary(op, *expr1, *expr2);
    }
    return *expr1;
}

Expression& Parser::parseMultiplication() throw(SyntaxError)
{
    // Mult = Simple { ( '*' | '/' ) Simple }.
    char op;
    Expression* expr1 = &parseSimple();
    while (lookahead_.is(Kind::Mult) || lookahead_.is(Kind::Div))
    {
        op = lookahead_.lexeme_[0];
        lookahead_ = scanner_.scan();
        Expression* expr2 = &parseSimple();
        expr1 = new Binary(op, *expr1, *expr2);
    }
    return *expr1;
}

Expression& Parser::parseSimple() throw(SyntaxError)
{
    // Simple = '(' Expr ')'| Name | ID '\''{'\''} '(' ID ')' | ID '(' NUM ')'.
    Expression*  expr = nullptr;
    std::string name;
    std::string variable;
    int         value;
    int         order = 0;
    if (lookahead_.is(Kind::LeftBrace))
    {
        expr = &parseExpression();
        match(Kind::RightBrace);
        return *expr;
    }
    else if (lookahead_.is(Kind::Identifier))
    {
        name       = lookahead_.lexeme_;
        lookahead_ = scanner_.scan();
        if (lookahead_.is(Kind::LeftBrace))
        {
            lookahead_ = scanner_.scan();
            if (lookahead_.is(Kind::Identifier))
            {
                variable   = lookahead_.lexeme_;
                lookahead_ = scanner_.scan();
                match(Kind::RightBrace);
                return *(new Name(name, variable));
            }
            else if (lookahead_.is(Kind::Number))
            {
                value      = std::stoi(lookahead_.lexeme_);
                lookahead_ = scanner_.scan();
                match(Kind::RightBrace);
                return *(new Eval(name, value));
            }
        }
        else if (lookahead_.is(Kind::Derive))
        {
            while (lookahead_.is(Kind::Derive))
            {
                ++order;
                lookahead_ = scanner_.scan();     
            }
            match(Kind::LeftBrace);
            variable = lookahead_.lexeme_;
            match(Kind::Identifier);
            match(Kind::RightBrace);
            return *(new Derive(name, variable, order));
        }
    }
    return *expr;
}

void Parser::parseNewLines(bool leading) throw(SyntaxError)
{
    // Newlinws = NL {NL}.
    if (!leading)
        match(Kind::NL);
    while (lookahead_.is(Kind::NL))
        lookahead_ = scanner_.scan();
}

void Parser::match(const Kind& kind) throw(SyntaxError)
{
    if (lookahead_.is(kind))
        lookahead_ = scanner_.scan();
    else
        throw SyntaxError("Syntax error in line: ", scanner_.line());
}

Parser::DSU Parser::dsu()
{
    Scanner scanner   = scanner_;
    Token   lookahead = lookahead_;

    if (lookahead.is(Kind::Print))
        return Parser::DSU::Statement;
    while (!lookahead.is(Kind::NL))
    {
        if (lookahead.is(Kind::Define))
            return Parser::DSU::Definition;
        if (lookahead.is(Kind::Assign))
            return Parser::DSU::Statement;
        lookahead = scanner.scan();
    }
    return Parser::DSU::Unknown;
}