# BoolInterpreter
Interpreter to evaluate the value of boolean expressions (true or false). The grammar that was provided for the language requires that the user enters a period at the end of their expression in order for the interpreter to know the end of their input. In addition, whitespace is treated as a delimiter and any amounts of whitespace can be used in between symbols. Note that the implication symbol, ->, consists of two characters and will only be considered valid if no whitespace occurs between the two characters otherwise an error will be thrown.<br> <br> The syntax of the language allows for the use of the following symbols: T, F, ->, v, ^, (, ). <br>


Note that "T" and "F" are symbols for true and false respectively, the arrow "->" refers to the implication sign, and "v" and "^" are the binary operators "or" and "and" respectively. The "(" and ")" are used to enclose a subexpression for precedence (refer to examples for use). As stated before the "." refers to the end of input and end of the boolean expression that was given.

Examples of valid uses: <br>
T ^ F.            (This is read at "True and False" which will evaluate to False) <br>
(T v F) -> F.     (Read as "True or False implies False". Evaluates to False) <br>
T    ->    T.     ("True implies True". Evaluates to True) <br>

Invalid uses: <br>
T  vv  T.         ("v" is a binary operator) <br>
t    ->  F.       ("t" is not a valid symbol. Use "T") <br>
T     -    >   F  (There cannot be any whitespace in between the implication symbol.) <br>

<br> NOTE:  I take no credit for the grammar of this language. The grammar provided is entirely credited to Dr. Watson at Texas Tech University. 
