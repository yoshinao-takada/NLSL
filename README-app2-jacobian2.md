# Objective and Jacobian Formulation for NLSLapp2.exe
## Objective
This application calculate a homography matrix.  
__P__<sub>K</sub> = H __P__<sub>J</sub>  
[ _X_<sub>K</sub> , _Y_<sub>K</sub> , _Z_<sub>K</sub> ]<sup>T</sup> = [  
&nbsp; &nbsp; _h_<sub>00</sub> , _h_<sub>01</sub> , _h_<sub>02</sub> ;  
&nbsp; &nbsp; _h_<sub>10</sub> , _h_<sub>11</sub> , _h_<sub>12</sub> ;  
&nbsp; &nbsp; _h_<sub>20</sub> , _h_<sub>21</sub> , _h_<sub>22</sub> ;  
]
[ _X_<sub>J</sub> , _Y_<sub>J</sub> , _Z_<sub>J</sub> ]<sup>T</sup>
<br>
_X_<sub>K</sub> = _h_<sub>00</sub> _X_<sub>J</sub> + _h_<sub>01</sub> _Y_<sub>J</sub> + _h_<sub>02</sub> _Z_<sub>J</sub> ,  
_Y_<sub>K</sub> = _h_<sub>10</sub> _X_<sub>J</sub> + _h_<sub>11</sub> _Y_<sub>J</sub> + _h_<sub>12</sub> _Z_<sub>J</sub> ,  
_Z_<sub>K</sub> = _h_<sub>20</sub> _X_<sub>J</sub> + _h_<sub>21</sub> _Y_<sub>J</sub> + _h_<sub>22</sub> _Z_<sub>J</sub> .  
_x_<sub>K</sub> (_h_<sub>20</sub> _X_<sub>J</sub> + _h_<sub>21</sub> _Y_<sub>J</sub> + _h_<sub>22</sub> _Z_<sub>J</sub>) = _h_<sub>00</sub> _X_<sub>J</sub> + _h_<sub>01</sub> _Y_<sub>J</sub> + _h_<sub>02</sub> _Z_<sub>J</sub> ,  
_y_<sub>K</sub> (_h_<sub>20</sub> _X_<sub>J</sub> + _h_<sub>21</sub> _Y_<sub>J</sub> + _h_<sub>22</sub> _Z_<sub>J</sub>) = _h_<sub>10</sub> _X_<sub>J</sub> + _h_<sub>11</sub> _Y_<sub>J</sub> + _h_<sub>12</sub> _Z_<sub>J</sub> ,  
0 = _h_<sub>00</sub> _X_<sub>J</sub> + _h_<sub>01</sub> _Y_<sub>J</sub> + _h_<sub>02</sub> _Z_<sub>J</sub> -
_h_<sub>20</sub> _x_<sub>K</sub> _X_<sub>J</sub> - _h_<sub>21</sub> _x_<sub>K</sub> _Y_<sub>J</sub> -
_h_<sub>22</sub> _x_<sub>K</sub> _Z_<sub>J</sub> , ----- (1)  
0 = _h_<sub>10</sub> _X_<sub>J</sub> + _h_<sub>11</sub> _Y_<sub>J</sub> + _h_<sub>12</sub> _Z_<sub>J</sub> -
_h_<sub>20</sub> _y_<sub>K</sub> _X_<sub>J</sub> - _h_<sub>21</sub> _y_<sub>K</sub> _Y_<sub>J</sub> -
_h_<sub>22</sub> _y_<sub>K</sub> _Z_<sub>J</sub> , ----- (2)  
<br>
Instantiating (1) and (2) with four homological points,
(_x_<sub>K</sub><sup>(0)</sup> , _y_<sub>K</sub><sup>(0)</sup>) -
(_X_<sub>J</sub><sup>(0)</sup> , _Y_<sub>J</sub><sup>(0)</sup> , _Z_<sub>J</sub><sup>(0)</sup>) , 
(_x_<sub>K</sub><sup>(1)</sup> , _y_<sub>K</sub><sup>(1)</sup>) -
(_X_<sub>J</sub><sup>(1)</sup> , _Y_<sub>J</sub><sup>(1)</sup> , _Z_<sub>J</sub><sup>(1)</sup>) ,
(_x_<sub>K</sub><sup>(2)</sup> , _y_<sub>K</sub><sup>(2)</sup>) -
(_X_<sub>J</sub><sup>(2)</sup> , _Y_<sub>J</sub><sup>(2)</sup> , _Z_<sub>J</sub><sup>(2)</sup>) ,
(_x_<sub>K</sub><sup>(3)</sup> , _y_<sub>K</sub><sup>(3)</sup>) -
(_X_<sub>J</sub><sup>(3)</sup> , _Y_<sub>J</sub><sup>(3)</sup> , _Z_<sub>J</sub><sup>(3)</sup>),  
eight simultaneous linear equation is obtainied.  
[ __0__<sup>8</sup> ]<sup>T</sup> = [  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(0)</sup> , _Y_<sub>J</sub><sup>(0)</sup> , _Z_<sub>J</sub><sup>(0)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(0)</sup> _X_<sub>J</sub><sup>(0)</sup>
, -_x_<sub>K</sub><sup>(0)</sup> _Y_<sub>J</sub><sup>(0)</sup>
, -_x_<sub>K</sub><sup>(0)</sup> _Z_<sub>J</sub><sup>(0)</sup> ;  
&nbsp; &nbsp;0 , 0 , 0 , _X_<sub>J</sub><sup>(0)</sup> , _Y_<sub>J</sub><sup>(0)</sup> , _Z_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _X_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _Y_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _Z_<sub>J</sub><sup>(0)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(1)</sup> , _Y_<sub>J</sub><sup>(1)</sup> , _Z_<sub>J</sub><sup>(1)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(1)</sup> _X_<sub>J</sub><sup>(1)</sup>
, -_x_<sub>K</sub><sup>(1)</sup> _Y_<sub>J</sub><sup>(1)</sup>
, -_x_<sub>K</sub><sup>(1)</sup> _Z_<sub>J</sub><sup>(1)</sup> ;  
&nbsp; &nbsp;0 , 0 , 0 , _X_<sub>J</sub><sup>(1)</sup> , _Y_<sub>J</sub><sup>(1)</sup> , _Z_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _X_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _Y_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _Z_<sub>J</sub><sup>(1)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(2)</sup> , _Y_<sub>J</sub><sup>(2)</sup> , _Z_<sub>J</sub><sup>(2)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(2)</sup> _X_<sub>J</sub><sup>(2)</sup>
, -_x_<sub>K</sub><sup>(2)</sup> _Y_<sub>J</sub><sup>(2)</sup>
, -_x_<sub>K</sub><sup>(2)</sup> _Z_<sub>J</sub><sup>(2)</sup> ;  
&nbsp; &nbsp;0 , 0 , 0 , _X_<sub>J</sub><sup>(2)</sup> , _Y_<sub>J</sub><sup>(2)</sup> , _Z_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _X_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _Y_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _Z_<sub>J</sub><sup>(2)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(3)</sup> , _Y_<sub>J</sub><sup>(3)</sup> , _Z_<sub>J</sub><sup>(3)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(3)</sup> _X_<sub>J</sub><sup>(3)</sup>
, -_x_<sub>K</sub><sup>(3)</sup> _Y_<sub>J</sub><sup>(3)</sup>
, -_x_<sub>K</sub><sup>(3)</sup> _Z_<sub>J</sub><sup>(3)</sup> ;  
&nbsp; &nbsp;0 , 0 , 0 , _X_<sub>J</sub><sup>(3)</sup> , _Y_<sub>J</sub><sup>(3)</sup> , _Z_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _X_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _Y_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _Z_<sub>J</sub><sup>(3)</sup>  
] [ _h_<sub>00</sub> , _h_<sub>01</sub> , _h_<sub>01</sub> , _h_<sub>10</sub> , _h_<sub>11</sub> ,
_h_<sub>12</sub> , _h_<sub>20</sub> , _h_<sub>21</sub> , _h_<sub>22</sub> ]<sup>T</sup>  .  
The last equation is a nonlinear equation,  
0 = &lt;__h__ , __h__&gt; - 1 .  
The first eight rows of Jacobian is the matrix part of the first eight equations. The last row of Jacobian is
2 * __h__ .  

The nine dimensional vector function __F__(__h__) is the objective function.  
__F__(__h__) = [  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(0)</sup> , _Y_<sub>J</sub><sup>(0)</sup> , _Z_<sub>J</sub><sup>(0)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(0)</sup> _X_<sub>J</sub><sup>(0)</sup>
, -_x_<sub>K</sub><sup>(0)</sup> _Y_<sub>J</sub><sup>(0)</sup>
, -_x_<sub>K</sub><sup>(0)</sup> _Z_<sub>J</sub><sup>(0)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(0)</sup> , _Y_<sub>J</sub><sup>(0)</sup> , _Z_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _X_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _Y_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _Z_<sub>J</sub><sup>(0)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(1)</sup> , _Y_<sub>J</sub><sup>(1)</sup> , _Z_<sub>J</sub><sup>(1)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(1)</sup> _X_<sub>J</sub><sup>(1)</sup>
, -_x_<sub>K</sub><sup>(1)</sup> _Y_<sub>J</sub><sup>(1)</sup>
, -_x_<sub>K</sub><sup>(1)</sup> _Z_<sub>J</sub><sup>(1)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(1)</sup> , _Y_<sub>J</sub><sup>(1)</sup> , _Z_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _X_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _Y_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _Z_<sub>J</sub><sup>(1)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(2)</sup> , _Y_<sub>J</sub><sup>(2)</sup> , _Z_<sub>J</sub><sup>(2)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(2)</sup> _X_<sub>J</sub><sup>(2)</sup>
, -_x_<sub>K</sub><sup>(2)</sup> _Y_<sub>J</sub><sup>(2)</sup>
, -_x_<sub>K</sub><sup>(2)</sup> _Z_<sub>J</sub><sup>(2)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(2)</sup> , _Y_<sub>J</sub><sup>(2)</sup> , _Z_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _X_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _Y_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _Z_<sub>J</sub><sup>(2)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(3)</sup> , _Y_<sub>J</sub><sup>(3)</sup> , _Z_<sub>J</sub><sup>(3)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(3)</sup> _X_<sub>J</sub><sup>(3)</sup>
, -_x_<sub>K</sub><sup>(3)</sup> _Y_<sub>J</sub><sup>(3)</sup>
, -_x_<sub>K</sub><sup>(3)</sup> _Z_<sub>J</sub><sup>(3)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(3)</sup> , _Y_<sub>J</sub><sup>(3)</sup> , _Z_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _X_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _Y_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _Z_<sub>J</sub><sup>(3)</sup> ;  
&nbsp; &nbsp;_h_<sub>00</sub> , _h_<sub>01</sub> , _h_<sub>01</sub> , _h_<sub>10</sub> , _h_<sub>11</sub> ,
_h_<sub>12</sub> , _h_<sub>20</sub> , _h_<sub>21</sub> , _h_<sub>22</sub>  
] [ _h_<sub>00</sub> , _h_<sub>01</sub> , _h_<sub>01</sub> , _h_<sub>10</sub> , _h_<sub>11</sub> ,
_h_<sub>12</sub> , _h_<sub>20</sub> , _h_<sub>21</sub> , _h_<sub>22</sub> ]<sup>T</sup> +  
[ 0, 0, 0, 0, 0, 0, 0, 0, -1 ]<sup>T</sup> <br> =
[ 0, 0, 0, 0, 0, 0, 0, 0, 0 ]<sup>T</sup> . ----- (1)  

## Jacobian
J = [
&nbsp; &nbsp; _X_<sub>J</sub><sup>(0)</sup> , _Y_<sub>J</sub><sup>(0)</sup> , _Z_<sub>J</sub><sup>(0)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(0)</sup> _X_<sub>J</sub><sup>(0)</sup>
, -_x_<sub>K</sub><sup>(0)</sup> _Y_<sub>J</sub><sup>(0)</sup>
, -_x_<sub>K</sub><sup>(0)</sup> _Z_<sub>J</sub><sup>(0)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(0)</sup> , _Y_<sub>J</sub><sup>(0)</sup> , _Z_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _X_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _Y_<sub>J</sub><sup>(0)</sup>
, -_y_<sub>K</sub><sup>(0)</sup> _Z_<sub>J</sub><sup>(0)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(1)</sup> , _Y_<sub>J</sub><sup>(1)</sup> , _Z_<sub>J</sub><sup>(1)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(1)</sup> _X_<sub>J</sub><sup>(1)</sup>
, -_x_<sub>K</sub><sup>(1)</sup> _Y_<sub>J</sub><sup>(1)</sup>
, -_x_<sub>K</sub><sup>(1)</sup> _Z_<sub>J</sub><sup>(1)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(1)</sup> , _Y_<sub>J</sub><sup>(1)</sup> , _Z_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _X_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _Y_<sub>J</sub><sup>(1)</sup>
, -_y_<sub>K</sub><sup>(1)</sup> _Z_<sub>J</sub><sup>(1)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(2)</sup> , _Y_<sub>J</sub><sup>(2)</sup> , _Z_<sub>J</sub><sup>(2)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(2)</sup> _X_<sub>J</sub><sup>(2)</sup>
, -_x_<sub>K</sub><sup>(2)</sup> _Y_<sub>J</sub><sup>(2)</sup>
, -_x_<sub>K</sub><sup>(2)</sup> _Z_<sub>J</sub><sup>(2)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(2)</sup> , _Y_<sub>J</sub><sup>(2)</sup> , _Z_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _X_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _Y_<sub>J</sub><sup>(2)</sup>
, -_y_<sub>K</sub><sup>(2)</sup> _Z_<sub>J</sub><sup>(2)</sup> ;  
&nbsp; &nbsp; _X_<sub>J</sub><sup>(3)</sup> , _Y_<sub>J</sub><sup>(3)</sup> , _Z_<sub>J</sub><sup>(3)</sup>
, 0 , 0 , 0 , -_x_<sub>K</sub><sup>(3)</sup> _X_<sub>J</sub><sup>(3)</sup>
, -_x_<sub>K</sub><sup>(3)</sup> _Y_<sub>J</sub><sup>(3)</sup>
, -_x_<sub>K</sub><sup>(3)</sup> _Z_<sub>J</sub><sup>(3)</sup> ;  
&nbsp; &nbsp; 0 , 0 , 0 , _X_<sub>J</sub><sup>(3)</sup> , _Y_<sub>J</sub><sup>(3)</sup> , _Z_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _X_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _Y_<sub>J</sub><sup>(3)</sup>
, -_y_<sub>K</sub><sup>(3)</sup> _Z_<sub>J</sub><sup>(3)</sup> ;  
&nbsp; &nbsp;2 _h_<sub>00</sub> , 2 _h_<sub>01</sub> , 2 _h_<sub>01</sub> , 2 _h_<sub>10</sub> , 2 _h_<sub>11</sub> ,
2 _h_<sub>12</sub> , 2 _h_<sub>20</sub> , 2 _h_<sub>21</sub> , 2 _h_<sub>22</sub>  
] . ----- (2)  

## References
[1] Antonio Criminisi, "Accurage Visual Metrology from Single and Multiple Uncalibrated Images", Springer-Verlag London 2001, ISBN 978-1-4471-1040-8, "4.1 Estimating the homography", p.41
The formulation above is mainly based on the book section but a little improved.

[2] https://en.wikipedia.org/wiki/Gauss%E2%80%93Newton_algorithm

[3] https://en.wikipedia.org/wiki/Newton%27s_method
