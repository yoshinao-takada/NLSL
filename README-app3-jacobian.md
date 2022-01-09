# Objective and Jacobian Formulation for NLSLapp3.exe
This sample is a very simple solution with a linear equation in spite that
Gauss-Newton solver is for non-linear equations.
## Objective
__F__(__X__) = M __X__ - __Y__ = [0] .  
M: 2x2 known matrix  
__X__ : unknown 2D variables  
__Y__ : known 2D vector  

## Jacobian
_d_ __F__(__X__) / _dx_<sub>0</sub> = [  
&nbsp; &nbsp; _m_<sub>00</sub> ;  
&nbsp; &nbsp; _m_<sub>10</sub> ]  
_d_ __F__(__X__) / _dx_<sub>1</sub> = [  
&nbsp; &nbsp; _m_<sub>01</sub> ;  
&nbsp; &nbsp; _m_<sub>11</sub> ]  
