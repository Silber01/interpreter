# CSIL Interpreter
Interpreter for a basic C-like language called CSIL (created for the purpose of the interpreter)
## Instructions
`print(<expression>);` \
`int <varName>;` \
`int <varName>; = <expression>;` \
`string <varName>;` \
`string <varName>; = <expression>;` \
`print(<expression>);` \
`input(<variable>);` \
`if(<expression>)` \
`while(<expression>);` 
## Data Types and Casting
There are two types of data: ints and strings. The data type of an expression is
based on the first term in the expression. For example, `"" + 10` will be casted as
a string, and `0 + "10"` will be casted as an int. This applies to nested expressions, so
`("" + 10) + (0 + "10")` will be calculated as `STRING + INT`, which is then casted as a string.

## Conditions and statements
all if and while statements must be proceeded by brackets. There is currently no `else if` support,
but can be done by using `else { if(...`.

## How to Use
Write the script in `script.txt`, and run the code by running `main.cpp`. `script.txt`
currently has the code for the fibonacci sequence as an example.
