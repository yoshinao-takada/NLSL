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
* Supply a derivative free solver: Nelder-Mead method
* Supply a non-derivative free solver: Gauss-Newton method, which requires multiple callbacks for Jacobian but very fast.

Therefore I kept following features as much as possible.
* Sticking to ISO-C 99
* Build with gnu make (It is sufficient and good for small simple projects.)
* Equipped with small linear algebraic matrix functions and sample main program.
* All functions and structures exposed from a source file are detailed commented.

## Source Files
Names | Description
--|--
NLSLapp.c | main source of sample applications for Nelder-Mead method (applications are switched by #define EQUSETUP  <number>)
NLSLapp2.c | main source of a sample application for Gauss-Newton method
NLSLsolver.c/.h | top level solver library functions of Nelder-Mead method
NLSLvars.c/.h | container struct of x independent variable vector, y dependent variable vector
NLSLutils.c/.h | simple array and pointer operations
NLSLmatrix.c/.h | algebraic matrix operations; not used in NLSLsolver, just for developing user objective functions.
NLSLgnsolver.c/.h | top level solver library functions of Gauss-Newton method
<br>

## Sample Codes in NLSLapp.c
### Sample 0: Simple 2nd order optimization

### Sample 1: three 2x2 matrices make three 2x2 matrix products.
Three independent variable matrices X<sub>0</sub>, X<sub>1</sub>, and X<sub>2</sub> are estimaged from the three products,  
&nbsp; &nbsp; &nbsp; &nbsp; Y<sub>0</sub> = X<sub>0</sub> X<sub>1</sub>  
&nbsp; &nbsp; &nbsp; &nbsp; Y<sub>1</sub> = X<sub>1</sub> X<sub>2</sub>  
&nbsp; &nbsp; &nbsp; &nbsp; Y<sub>2</sub> = X<sub>2</sub> X<sub>0</sub>  
Targets of the elements of Y<sub>0</sub>, Y<sub>1</sub>, and Y<sub>2</sub> are given as
Y<sub>T0</sub> , Y<sub>T1</sub> , and Y<sub>T2</sub>.
Varying X<sub>0</sub> , X<sub>1</sub> , X<sub>2</sub> elements,
calculating Y<sub>0</sub> , Y<sub>1</sub> , Y<sub>2</sub> elements, Y<sub>i</sub> and T<sub>Ti</sub>
are compared to update X<sub>0</sub> , X<sub>1</sub> , X<sub>2</sub> elements for next iteration.  
Iteration is terminated by its limit or convergence of X or Y.

### Sample 1: code details
Initial values of X and Y are shown below.
```
#if (EQUSETUP == 0)
#define YTARGET { -1.4f, -1.8f, -10.6f, -24.4f, 1.0f, -26.1f, 0.0f, 15.2f, 1.2f, -4.9f, 11.56f, 0.55f }
#define XIDEAL { 1.0f, 2.2f, -3.2f, 0.5f, 3.0f, 7.0f, -2.0f, -4.0f, -2.0f, -1.0f, 1.0f, -3.3f }
#define XINITIAL { 2.0f, 2.2f, -3.2f, 0.5f, 3.0f, 7.0f, -2.0f, -4.0f, -2.0f, -1.0f, 0.0f, 0.0f }
#define PARAMS { 0.0f }
```
`YTARGET` consists of elements of three 2x2 matrices of objective function target values,  
Representing in gnu octave style where each matrix row is terminated by a semicolon,  
Y<sub>T0</sub> = [  
&nbsp; &nbsp; -1.4, -1.8;  
&nbsp; &nbsp; -10.6, -24.4  
]  
Y<sub>T1</sub> = [  
&nbsp; &nbsp; 1.0, -26.1;  
&nbsp; &nbsp; 0.0, 15.2  
]  
Y<sub>T2</sub> = [  
&nbsp; &nbsp; 1.2, -4.9;  
&nbsp; &nbsp; 11.56, 0.55  
]  

`XIDEAL` consists of elements of three 2x2 marices of ideal independent variables to make objective function results equal to their target values which are shown as `YTARGET` above.  
X<sub>IDEAL0</sub> = [  
&nbsp; &nbsp; 1.0, 2.2;  
&nbsp; &nbsp; -3.2, 0.5  
]  
X<sub>IDEAL1</sub> = [  
&nbsp; &nbsp; 3.0, 7.0;  
&nbsp; &nbsp; -2.0, -4.0  
]  
X<sub>IDEAL2</sub> = [  
&nbsp; &nbsp; -2.0, -1.0;  
&nbsp; &nbsp; 1.0, -4.0  
]  
`XIDEAL` is not used actually. It is written in the code just for reference information.

`XINITIAL` consists of elements of three 2x2 matrices of initial independent variables.
The objective function has multiple local minima. Then the example problem start at a point
a little close to the ideal point.  
X<sub>INITIAL0</sub> = [  
&nbsp; &nbsp; 2.0, 2.2;  
&nbsp; &nbsp; -3.2, 0.5  
]  
X<sub>INITIAL1</sub> = [  
&nbsp; &nbsp; 3.0, 7.0;  
&nbsp; &nbsp; -2.0, -4.0  
]  
X<sub>INITIAL2</sub> = [  
&nbsp; &nbsp; -2.0, -1.0;  
&nbsp; &nbsp; 0.0, 0.0  
]  

## Sample