# Galois
 It's a simple language where single variable polynomial is a base type.
 It's allow addition, multiplication, derivation, counting operations for polynomials. And has basic controlling structures.
 
 
 Language name associated with very gifted young French mathematician Évariste Galois, who was killed at the age of 20 in a duel. He proved that it is impossible to solve a fifth-degree polynomial (or a polynomial of any higher degree) using radicals by studying permutation groups associated to polynomials.


__Language Usage__
````
# Definitions
P1(x) := -x^3 + 3x^2 - 1
P2(x) := 4x + 7
# Statements
P3(x) = (P1(x) + P2(x)) * P2(x)'
P4(x) = P3(2)'' - P2(x)
print P3(x)
````

__Language Grammar__
````
Program    = [NewLines] { (Definition | Statement) NewLines }.
Definition = Name ':=' Polynom.
Name       = ID '(' ID ')'.
Polynom    = ['+'|'-'] Element { ('+'|'-') Element }.
Element    = NUM | [NUM] ID ['^' NUM].
Statement  = Name '=' Expr | PRINT Expr.
Expr       = Mult   { ( '+' | '-' ) Mult }.
Mult       = Simple { ( '*' | '/' ) Simple }.
Simple     = '(' Expr ')'
           | Name
           | ID '\''{'\''} '(' ID ')'
           | ID '(' NUM ')'.
Newlinws   = NL {NL}.
````
