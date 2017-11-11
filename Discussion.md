# Բազմանդամների լեզու և ինտերպրետատոր

Իրականացնել մեկ փոփոխականի բազմանդամը որպես բազային տիպ ունեցող ալգորիթմական լեզու։ Ապահովել բազմանդամների գումարման, բազմապատկման, ածանցման, հաշվարկման գործողություններ։

Բազմանդամի ու որոշ գործողությունների սահմանումը այս լեզվով ունի հետևյալ տեսքը․
````
poly P1 
{
   degree       = 4
   coefficients = {1, 0, 2, -3, 7}
}
print P1
````
Օրինակ այստեղ կտպվի ````x^4 + 2x^2 - 3x + 7```` արտահայտությաունը։
````
poly P2
{
   degree       = 2
   coefficients = {3, 0, -2}
}
print P2
````
Այստեղ՝ ````3x^2 - 2```` արտահայտությաունը:
````
poly P3 = P1 + P2
evaluate P3 at 4
````
Այստեղ կստանանք ````P3```` բազմանդամի արժեքը ````4```` կետում։

# Լեզվի քերականությունը

````
Program      = {[Newlines] (Polynom | Operations) Newlines}.
Polynom      = 'poly' ID '{' Degree Coefficients '}'.
Degree       = 'degree' '=' NUMBER.
Coefficients = 'coefficients' '=' '{' NUMBER {',' NUMBER} '}'.
Operations   = (Add | Mult | Derive | Eval | Print) Newlines.
Print        = 'print' Expr.
Factor       = NUMBER | ID | '(' Expr ')'.
Expr         = NUMBER | ID.
Derive       = Factor '՛' {'՛'}.
Eval         = 'evaluate' Derive 'at' NUMBER.
Mult         = Derive {* Derive}.
Add          = Mult {(+ | -) Mult}.
Newlinws     = NL {NL}.
````

# Առաջարկ

__Բազմանդամի ներկայացման մասին։__ Ցանկալի կլիներ, որ բազմանդամը ներկայացվեր «բնական» տեսքով։ Օրինակ, վերը բերված օրինակում.
````
poly P2 {
    degree       = 2
    coefficients = {3, 0, -2}
}
print P2
````
պարզապես ներկայացված է `P2(x) = 3x^2 - 2` բազմանդամը։ Ինչո՞ւ լեզվում չգրել հենց այս տեսքով։ Կարծում եմ, դժվար կգտնվի մի օգտագործող, ով բազմանդամը կգրի ոչ թե «բնական» տեքսով, այլ բերված քերականությամբ։


# Փոփոխություններից հետո
__Ներկայացման տեսքը__
````
// Definitions
P1(x) = -x^3 + 3x^2 - 1
P2(x) = 4x + 7
// Operations
P3(x) = (P1(x) + P2(x)) * P2(x)'
P4(x) = P3(2)'' - P2(x)
print P3(x)
````

__Լեզվի քերականությունը__

````
Program  = {[Newlines] (Def | Op) Newlines}.
Def      = Name '=' Body.       
Name     = ID '(' (ID | NUM) ')'. 
Body     = Elem {(+ | -) Elem}.
Elem     = NUM | [NUM] ID ['^' NUM].
Op       = (Name '=' (Add | Mult | Derive | Eval)) | Print Newlines.
Factor   = Name | '(' Name ')'.
Unary    = ['-'] Factor.
Derive   = Unary {'''}.
Eval     = Derive.
Mult     = Eval {(* | /) Eval}.
Add      = Mult {(+ | -) Mult}.
Print    = 'print' Name.
Newlinws = NL {NL}.
````

__Նոր լրացումներ և ուղղումներ__


Ծրագիրը սահմանումների ու հրամանների հաջորդականություն է․

````
Program = [NewLines] { (Definition | Statement) NewLines }.
````

Սահմանման միջոցավ բազմանդամը կապվում է որևէ անուն․

````
Definition = Name '=' Polynom.
````

Անունն իր մեջ պարունակում է նաև բազմանդամի փոփոխականը․

````
Name = ID '(' ID ')'.
````

Բազմանդամն իրար `+` կամ `-` սիմվոլներով միացած տարրերի հաջորդականություն է․

````
Polynom = ['+'|'-'] Element '{' ('+'|'-') Element }.
Element = NUM | [NUM] ID ['^' NUM].
````

Հրամաններից մեկը բազմանդամի արտածումն է․

````
Statement = PRINT Expr
    | .... .
````

Բազմանդամները կարող են գումարվել իրար, բազմապատկվել մեկը մյուսի հետ, կարելի է հաշվել բազմանդամի ածանցյալը, կարելի է հաշվել բազմանդամի արժեքը տրված թվի համար․

````
Expr = Mult { '+' Mult }.
Mult = Simple { '*' Simple }.
Simple = '(' Expr ')'
       | Name
	   | ID '\'' '(' ID ')'
	   | ID '(' NUM ')'.
````

__Քերականությունը ուղղումներից հետո__
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
