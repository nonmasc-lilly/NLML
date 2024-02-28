# nonmasc lillys macro language

NLML is a language designed to process files into other files.

## Syntax

- quotes `""` denote a string
- `x` before a number denotes a hexadecimal number
- `o` before a number denotes octal
- `d` before a number denotes decimal
- `b` before a number denotes binary
- unmarked numbers are hexadecimal
- `=>` between two tokens defines a macro between them
- a macro consists of two datapoints (in and out) at runtime the macro looks for instances of in inside a file and replaces it with out
- `!` denotes a command
- `!intsize` changes the size of integer values, default is 1 byte, takes one int argument equal to the number of bytes to use.



