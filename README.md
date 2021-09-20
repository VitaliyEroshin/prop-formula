# PropFormulaLang
## How to make it work...
Well, first you need to import the class into your c++ project like so:\
```#include "PropositionalFormulaClass.cpp```\
Next, you need to create propositional formula in your code:\
```porp::PropositionalFormula formula;```\
Now you can set the formula with ```formula.read(string)``` or get a string that represents it ```string s = formula.show()```
## For what?
You can evaluate formula on function that returns bool value by variable name\
``` formula.evaluate(f)``` where f is ```bool f(std::string)```, or, you can use special struct I created in porputil.
```
proputil::Evaluation v;
v.push("a1", true);
formula.evaluate(v); // returns bool
```
Also there are ```formula.sat()``` and ```formula.taut()``` that check whether the forumla is SAT or TAUT.

## Syntax
There is several main rules how to set the formula:\
Let ```s1, s2``` be correct propositional formulas or variables, ```*``` is logic operation (or, and, implies or their unicode symbols)
1. ```(s1)``` - is correct propositional formula
2. ```s1 * s2``` - is correct propositional formula
3. ```not(s1)``` or ```¬s1``` - is also correct formula

Variables must be strings in one of these formats:
1. Sequence of small latin symbols
2. Sequence of small latin symbols and then sequence of digits

Example: ```abacaba123```, ```alpha``` are correct variable names, ```HHs2HA``` is not correct.\
Not following this rules can cause undefined behavior or errors.

## How it works?
When you call ```formula.read(string)``` it recursively splits the string into nodes and leaves. As a result you get a tree, where
every leaf is a variable and every node is combination of two or one nodes or leaves (or combined). In every node has it's own defined operation.\
```formula.show()``` just recursively builds the string from that tree.
