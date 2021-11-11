# NLSL: **N**on**L**inear **s**o**l**ver using Nelder-Mead algorithm
## Objective and Design Consideration
There are many applications mainly based on linear algebraic analysis but requiring
small part of nonlinear equation solver. NLSL interface is designed suitable to
treating linear algebraic vectors and matrices.  
Another consideration is ease of design change and understanding internal structures.
I considered following issues.
* Portability: All the sorce codes are written in ISO-C 99 and does not depends on third party OSS.
* Modularity: In many cases, small OSS mathematical programs are not well modularized and not easy to understand.
* Documentation: Good documentation and adequate sample codes helps users to understand the code.

Therefore I kept following features as much as possible.
* Sticking to ISO-C 99
* Build with gnu make (It is sufficient and good for small simple projects.)
* Accompanied with small linear algebraic matrix functions and sample main program.
* All functions and structures exposed from a source file are detailed commented.

## Source Files
Names | Description
--|--
NLSLapp.c | main source of sample applications (applications are switched by #define EQUSETUP  <number>)
NLSLsolver.c/.h | top level solver library functions
NLSLvars.c/.h | container struct of x independent variable vector, y dependent variable vector
NLSLutils.c/.h | simple array and pointer operations
NLSLmatrix.c/.h | algebraic matrix operations; not used in NLSLsolver, just for developing user objective functions.
<br>

## Sample Codes
### Sample 0: Simple 2nd order optimization

### Sample 1: three 2x2 matrices make three 2x2 matrix products.
Three independent variable matrices X<sub>0</sub>, X<sub>1</sub>, and X<sub>2</sub> are estimaged from the three products,  
Y<sub>0</sub> = X<sub>0</sub> X<sub>1</sub>  
Y<sub>1</sub> = X<sub>1</sub> X<sub>2</sub>  
Y<sub>2</sub> = X<sub>2</sub> X<sub>0</sub>  
Targets of the elements of Y<sub>0</sub>, Y<sub>1</sub>, and Y<sub>2</sub> are given as
Y<sub>T0</sub>, Y<sub>T1</sub>, and Y<sub>T2</sub>.
Varying X<sub>i</sub> elements, calculating Y<sub>i</sub> elements, Y<sub>i</sub> and T<sub>Ti</sub>
are compared to update X<sub>i</sub> elements for next iteration.  
