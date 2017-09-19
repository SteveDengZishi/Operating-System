# Two-pass linker

* Created by Zishi Deng (Steve)
* Written in C++

## Installation

#### Building from Source
1. Clone the repository and put input file to your root folder
2. Compile in terminal using g++ -o twoPassLinker twoPassLinker.cpp -std=c++11
3. ./twoPassLinker << EOF
   > input-1
   > EOF

## Features

* Resolve external addresses and relocate relative addresses
* If a symbol is defined but not used, print a warning message and continue
* If a symbol is multiply defined, print and error message and use the value given in the first definition
* If a symbol is used but not defined, print an error message and use the value zero
* If an address appearing in a definition exceeds the size of the module, print an error message and treat address as 0 (relative).
* If an immediate address appears on a use list, print an error message and treat the address as external
* If an external address is not on a use list, print an error message and treat it as an immediate address



