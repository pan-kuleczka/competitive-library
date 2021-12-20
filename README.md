# competitive-library
### Library with competitive algorithms and utilities in C++
### Files are not intended to be used as standard .h/.cpp pairs. You should copy the appropriate structs or definitions instead.
## 1. Compilation options (for Linux)
- Basic compilation
```bash
g++ -std=c++17 -O3 -Wall -Wextra -Wshadow -Wfloat-equal -Wconversion
```
- Compilation for debugging and sanitization
```bash
g++ -std=c++17 -Wall -Wextra -Wshadow -Wfloat-equal -Wconversion -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -pedantic -O3
```
- Compilation to Windows .exe
```bash
x86_64-w64-mingw32-g++ -std=c++17 -O3 -Wall -Wextra -Wshadow -Wfloat-equal
```
## 2. Functionality list
- Aho Corasick  
    Check for occurences of multiple words
- Dinic's Algorithm  
    Calculate the maximum flow in a directed/undirected graph
- Factorization  
    Check if a number is prime (Miller-Rabin primality test), factorize it using the Pollard's Rho Algorithm
- FFT  
    FFT a polynomial, multiply polynomials
- Geometry  
    N-dimensional vectors
- Suffix Automaton  
    Check if a word is a suffix
