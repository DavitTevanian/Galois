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
