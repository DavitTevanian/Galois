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
P1(x) = -x^3 + 3x^2 - 1
P2(x) = 4x + 7
P3(x) = (P1(x) + P2(x)) * P2(x)՛
P4    = P3(2)՛՛
P5(x) = P2(x)^2
print P4
print P3(x)
````

__Լեզվի քերականությունը__

````
Program    = {[Newlines] (Statement | Print) Newlines}.
Print      = 'print' Poly.
Poly       = (ID ['(' LETTER | NUMBER')']).
Statement  = Poly '=' (Expr | Operations).
Expr       = NUMBER | LETTER.
Factor     = Expr | '(' Expr ')'.
Unary      = ['-'] Factor.
Power      = Unary ['^' NUMBER].
Mult       = Power {Power}.
Add        = Mult  {(+ | -) Mult}.
Operations = (Padd | Pmult | Pderive | Ppower | Peval) Newlines.
Pexpr      = Poly.
Pfactor    = Pexpr | '(' Pexpr ')'.
Punary     = ['-'] Pfactor.
Pderive    = Punary {'՛'}.
Peval      = Pderive.
Pmult      = Peval {(* | /) Peval}.
Padd       = Pmult {(+ | -) Pmult}.
Newlinws   = NL {NL}.
````
