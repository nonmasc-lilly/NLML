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
- `\` before a character in a string denotes an escape sequence the escape sequences are as follows:
  + `\\`   becomes `\`
  + `\n`   becomes `\xA`
  + `\r`   becomes `\xD`
  + `\a`   becomes `\x7`
  + `\f`   becomes `\xC`
  + `\t`   becomes `\x9`
  + `\v`   becomes `\xB`
  + `\b`   becomes `\x8`
  + `\'`   becomes `'`
  + `\"`   becomes `"`
  + `\xnn` becomes the hex of `nn`

## installation

to install nlml head to the C folder and run make install
this will download the program executable into usr/local/bin
under the name `nmc`
