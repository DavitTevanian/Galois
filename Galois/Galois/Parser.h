#pragma once
#pragma warning( disable : 4290 ) 
#include "Scanner.h"
#include "Engine.h"

class Parser
{
public:
    Parser(const std::string& text);
    Engine& parse()                         throw(SyntaxError);
                                                
private:                                        
    Definition& parseDefinition()           throw(SyntaxError);
    Name&       parseName()                 throw(SyntaxError);
    Polynom&    parsePolynom()              throw(SyntaxError);
    Element&    parseElement()              throw(SyntaxError);
                                            
    Statement&  parseStatement()            throw(SyntaxError);
    Expression& parseExpression()           throw(SyntaxError);
    Expression& parseMultiplication()       throw(SyntaxError);
    Expression& parseSimple()               throw(SyntaxError);
                                        
    void        parseNewLines(bool leading) throw(SyntaxError);    
    void        match(const Kind& kind)     throw(SyntaxError);

    enum class DSU { Definition, Statement, Unknown };
    DSU         dsu();
private:
    
    Scanner  scanner_;
    Token    lookahead_;
};