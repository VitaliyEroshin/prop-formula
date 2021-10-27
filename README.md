# Math Logic & Theory of Algorithms
## How to make it work?
Well, first you need to import the class into your c++ project like so:\
```#include "./source/class.h"```\
Next, you need to create the formula in your code:\
```form::Formula formula;```\
Now you can set the formula with ```formula.read(string)``` or get a string that represents it ```string s = formula.show()```

## Syntax
There is several main rules how to set the formula:\
Let ```s1, s2``` be correct formulas or variables, ```*``` is logic operation (or, and, implies or their unicode symbols)
1. ```(s1)``` - is correct formula
2. ```s1 * s2``` - is correct formula
3. ```not(s1)``` or ```¬s1``` - is also correct formula

Variables must be strings in one of these formats:
1. Sequence of small latin symbols
2. Sequence of small latin symbols and then sequence of digits

Example: ```abacaba123```, ```alpha``` are correct variable names, ```HHs2HA``` is not correct.\
Not following this rules can cause undefined behavior or errors.

## What can I do with that formula?
You can evaluate formula on function that returns bool value by variable name\
``` formula.eval(f)``` where f is ```bool f(std::string)``` that returns variable value by it's name, or, you can use special struct I created in ```util.cpp```.
```
util::Evaluation v;
v.push("myvar", true);
formula.eval(v);
```

## SAT and TAUT
You can easily check if your formula SAT or TAUT like so:
```
formula.sat();      // returns true if sat
formula.taut();     // returns true if taut
```
Also you can put a flag as an argument to print the time of processing
```
formula.sat(true);  // returns true if sat, prints calculation time
```

## Ramsey numbers
That function can check if R is ramsey number R(n, m).

We are given number R and pair of number n, m. Function checks if ramsey number R(n, m) is R.

Firstly, we need to include header files.
```
#include "./tasks/ramsey.h"
```
Secondly, in our program we call the function like so:
```
fnc::ramsey(R, n, m);
```
The result of the function is bool value. True if R(n, m) = R and false else.
Also you can output processing time in milliseconds:
```
fnc::ramsey(R, n, m, true);
```

## Chromatic numbers
That function checks if you can color your graph in N colors and there won't be same colors at the end of the edge.

Firstly, include the header:
```
#include "./tasks/chromatic_number.h"
```

Now you can check if N can be chromatic number of the graph
```
fnc::color({{0, 1}, {1, 2}}, 2);
```
Here, {{0, 1}, {1, 2}} is a vector of pairs (```vector<pair<int, int>>```), where pairs are edges.
And you can add ```true``` flag as last argument to output the time.
