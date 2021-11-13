# Overview
## Simplex vertices
Vertices in N-dimensional space is stored in `NLSLsolver_t::vertices`. The elements are used for
the purposes listed below.

Vertex index | Purpose
--|--
`0..conf.cx - 1` | vertices of the moving simplex
`conf.cx` | worst point under evaluation, rejection candidate
`indexR` | reflection point
`indexE` | expansion point
`indexOC` | outside contraction point
`indexIC` | inside contraction point
`indexCentroid` | centroid of the simplex vertices
<br>

## Main loop  
It is implemented in `NLSLsolver_exec()`. Branching and replacing the worst point operation are
implemented in the loop and the function is complicated. The algorithm of branching and replacing the worst point is shown below.  
<br>
**[Algorithm pseudo code]**  
In the descriptions below, **evaluate** means calculating `y` by the objective function and calaculate L2 norm of
a distance between the `y` and `Y`<sub>`TARGET`</sub> .  
index `0..N-1`: index of vertices of simplex vertex  
index `N`: index of the worst vertex which is the candidate of rejection  
`V[i]`: `i`-th vertex, where `i = 0..N`  
`R`: reflection ponit  
`E`: expansion point  
`OC`: outside contraction point  
`IC`: inside contraction ponit  
`C`: centroid of `V[0]..[N-1]`   
`V[i] (i = 0..N), R, E, OC, IC,` and `C` are same struct containing  
&nbsp; &nbsp; &nbsp; &nbsp; `x`: vertex coordinate which is independent variables of the objective function.  
&nbsp; &nbsp; &nbsp; &nbsp; `y`: objective function result vector, dependent variables corresponding to `x`.  
&nbsp; &nbsp; &nbsp; &nbsp; `evalResult`: L2 norm of distance between `y` and `Y`<sub>`TARGET`</sub>  
Symbolic vertex names like below can be applied after sorting vertices;  
&nbsp; &nbsp; &nbsp; &nbsp; Index `worst = N` which is the candidate of rejection.  
&nbsp; &nbsp; &nbsp; &nbsp; Index `worst2 = N - 1` which is the last vertex of the moving simplex.  
<br>
**Start** :  
&nbsp; Set `V[0].x = X`<sub>`INITIAL`</sub> and evaluate `V[0].evalResult`.  
&nbsp; Fill `V[1..N].x`with adequately scattered coordinates surrounding `X`<sub>`INITIAL`</sub>
and evaluate `V[1..N].evalResult`.  
**LoopEntry** :  
&nbsp; sort `V[0..N]` by their `evalResult` for `V[0].evalResult` &le; `...` &le; `V[0].evalResult`  
&nbsp; If `x` or `y` converged, exit loop.  
&nbsp; Set `C.x` to the centroid of `V[0..N].x` and evaluate `C`.  
&nbsp; Set `R`, `E`, `OC`, and `IC` and evaluate them.  
&nbsp; &nbsp; &nbsp; &nbsp; Note: It is redundant to evaluate all the candidate points before the following conditional branches.  
&nbsp; &nbsp; &nbsp; &nbsp; Note: The code becomes simpler.  
&nbsp; If (`V[0].evalResult` &le; `R.evalResult`) `AND`(`R.evalResult` &lt; `V[worst2].evalResult`)  
&nbsp; Then { `swap(R, V[worst])` and goto LoopEntry }  
&nbsp; Else if (`R.evalResult` &lt `V[0].evalResult`) then  
&nbsp; &nbsp; &nbsp; &nbsp; If (`E.evalResult` &le `R.evalResult`)  
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Then { `swap(E, V[worst])` and goto LoopEntry }  
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Else { `swap(E, V[worst])` and goto LoopEntry }  
&nbsp; Else if (`V[worst].evalResult` &le; `R.evalResult`) Then  
&nbsp; &nbsp; &nbsp; &nbsp; If ((`R.evalResult` &lt; `V[worst].evalResult`) `AND` (`OC.evalResult` &le; `R.evalResult`)  
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Then { `swap(OC, V[worst])` and goto LoopEntry }  
&nbsp; &nbsp; &nbsp; &nbsp; Else If ((`V[worst].evalResult` &le `R.evalResult`) `AND` (`IC.evalResult` & &lt; `V[worst].evalResult`))  
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Then { `swap(IC, V[worst])` and goto LoopEntry }  
&nbsp; Else { `shrink(V[1..N])`, evaluate them and goto LoopEntry }  
**LoopEnd**  
<br>

## Convergence check
Convergence check is implmeneted in the main loop and is executed just after sorting vertices.  
The main loop is terminated by one of the three conditions,  
1. Iteration down counter is equal to zero,  
2. L2 norm of distance between `V[0].y` and `Y`<sub>`TARGET`</sub>,  
3. Variatioin of coordinates of vertices, `V[0]`, `V[worst]`, and the centroid becomes less than their threshold.  
The convergence check of `y` is implemented in `NLSLsolver_yconverged()` in `NLsolver.c`.  
The convergence check of `x` is implemented in `NLSLsolver_xconverged()` in `NLsolver.c`.  

## Overview of procedure to implement an application
### 1. Implement objective function
The function type prototype is declared as `NLSL_OBJECTIVE` in `NLSLsolver.h`. Please refer to `objective()` in
`NLSLapp.c` for details.
### 2. Set vector dimensions and create a solver object.
`cx`, `cy`, `cp` : vector dimensions of independent variables, dependent variables, and 
constant parameters of the objective function.  
### 3. Set constant parameters in `NLSLsolverconf_t`
Retrieving a pointer to the configuration object instance in the solver instance. Then set their values.  
### 4. Iterate optimization
Execute `NLSLsolver_exec()`.  
### 5. Retrieve results
`NLSLsolver_t` has result retrieving functions.
Name | Description
--|--
`NLSLsolver_xfinal()` | retrieve `x` after convergence
`NLSLsolver_yfinal()` | retrieve `V[0].y` after convergence
`NLSLsolver_evalfinal()` | retrieve distance of `y` - `Y`<sub>TARGET</sub> after convergence
`NLSLsolver_status()` | the status code shows the reason why the loop was terminated.
<br>
