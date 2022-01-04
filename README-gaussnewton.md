# Gauss-Newton Solver Formulation
## Single Variable Newton Method
Objective: Get _x_ satisfying  
f(_x_) = 0 ----- (1)  
_x_<sub>_i_</sub> does not satisfy the condition.  
Assuming that _x_<sub>_i_+1</sub> = _x_<sub>_i_</sub> + _&delta;x_<sub>_i_</sub> should satisfy the
condition  
f(_x_<sub>_i_</sub> + _&delta;x_<sub>_i_</sub>) = 0 . ----- (2)  
Taylor expansion of (2) is  
f(_x_<sub>_i_</sub>) + _&delta;x_<sub>_i_</sub> (_d_ f(_x_<sub>_i_</sub>) / _dx_) &approx; 0 , ----- (3)  
then  
_&delta;x_<sub>_i_</sub> = -f(_x_<sub>_i_</sub>) / (_d_ f(_x_<sub>_i_</sub>) / _dx_), ----- (4)  
_x_<sub>_i_+1</sub> = _x_<sub>_i_</sub> - f(_x_<sub>_i_</sub>) / (_d_ f(_x_<sub>i</sub>) / _dx_) ----- (5)

## Multi-Variable Newton Method
Replace _x_<sub>_i_</sub> and f(_x_<sub>_i_</sub>) with vectors and replace _d_ f(_x_<sub>i</sub>) / _dx_
with a Jacobian, (5) is rewritten as  
__X__<sub>_i_+1</sub> = __X__<sub>_i_</sub> - J(__F__(__X__<sub>_i_</sub>))<sup>-1</sup> __F__(__X__<sub>_i_</sub>) ,
----- (6)  
where J(__F__(__X__<sub>_i_</sub>)) is a Jacobian of __F__(__X__<sub>_i_</sub>) .

## Gauss-Newton Method
Gauss-Newton method is an extension of multi-variable Newton method using least-mean-square method which allows
that a dimension of __F__( ) is larger than a dimension of __X__ . (3) is rewritten as  
J( __F__(__X__<sub>_i_</sub>) ) __&delta;__ __X__<sub>_i_</sub> = -__F__(__X__<sub>_i_</sub>) . ----- (7)  
Applying least-mean-square method,  
J( __F__(__X__<sub>_i_</sub>) )<sup>T</sup> J( __F__(__X__<sub>_i_</sub>) ) __&delta;__ __X__<sub>_i_</sub> =
-J( __F__(__X__<sub>_i_</sub>) )<sup>T</sup> __F__(__X__<sub>_i_</sub>) , ----- (8)  
__&delta;__ __X__<sub>_i_</sub> =
-{J( __F__(__X__<sub>_i_</sub>) )<sup>T</sup> J( __F__(__X__<sub>_i_</sub>) )}<sup>-1</sup>
J( __F__(__X__<sub>_i_</sub>) )<sup>T</sup> __F__(__X__<sub>_i_</sub>) . ----- (9)  
__X__<sub>_i_+1</sub> = __X__<sub>_i_</sub> + __&delta;__ __X__<sub>_i_</sub>  
&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;  = __X__<sub>_i_</sub> -
{J( __F__(__X__<sub>_i_</sub>) )<sup>T</sup> J( __F__(__X__<sub>_i_</sub>) )}<sup>-1</sup>
J( __F__(__X__<sub>_i_</sub>) )<sup>T</sup> __F__(__X__<sub>_i_</sub>) . ----- (10)  