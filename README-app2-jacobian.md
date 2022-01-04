# Objective and Jacobian formulation for NLSLapp2.exe
## Objective
Unknowns are three matrices: X<sub>0</sub>, X<sub>1</sub>, X<sub>2</sub> , which are  
X<sub>0</sub> = [ _x_<sup>(0)</sup><sub>00</sub> , _x_<sup>(0)</sup><sub>01</sub> ;
_x_<sup>(0)</sup><sub>10</sub> , _x_<sup>(0)</sup><sub>11</sub> ] ,  
X<sub>1</sub> = [ _x_<sup>(1)</sup><sub>00</sub> , _x_<sup>(1)</sup><sub>01</sub> ;
_x_<sup>(1)</sup><sub>10</sub> , _x_<sup>(1)</sup><sub>11</sub> ] ,  
X<sub>2</sub> = [ _x_<sup>(2)</sup><sub>00</sub> , _x_<sup>(2)</sup><sub>01</sub> ;
_x_<sup>(2)</sup><sub>10</sub> , _x_<sup>(2)</sup><sub>11</sub> ] .  
The objective is  
X<sub>0</sub> X<sub>1</sub> - Y<sub>0</sub> = [0] , ----- (1)  
X<sub>1</sub> X<sub>2</sub> - Y<sub>1</sub> = [0] , ----- (2)  
X<sub>2</sub> X<sub>0</sub> - Y<sub>2</sub> = [0] , ----- (3)  
where Y<sub>0</sub> , Y<sub>1</sub> and Y<sub>2</sub> are fixed known matrices.  

## Jacobian
The 2nd terms (1), (2), and (3) becomes 0s by differentiation.  
X<sub>0</sub> X<sub>1</sub> = [  
&nbsp; &nbsp;  _x_<sup>(0)</sup><sub>00</sub>  _x_<sup>(1)</sup><sub>00</sub> +
_x_<sup>(0)</sup><sub>01</sub>  _x_<sup>(1)</sup><sub>10</sub> ,
&nbsp; &nbsp;  _x_<sup>(0)</sup><sub>00</sub>  _x_<sup>(1)</sup><sub>01</sub> +
_x_<sup>(0)</sup><sub>01</sub>  _x_<sup>(1)</sup><sub>11</sub> ;  
&nbsp; &nbsp;  _x_<sup>(0)</sup><sub>10</sub>  _x_<sup>(1)</sup><sub>00</sub> +
_x_<sup>(0)</sup><sub>11</sub>  _x_<sup>(1)</sup><sub>10</sub> ,
&nbsp; &nbsp;  _x_<sup>(0)</sup><sub>10</sub>  _x_<sup>(1)</sup><sub>01</sub> +
_x_<sup>(0)</sup><sub>11</sub>  _x_<sup>(1)</sup><sub>11</sub>  
] ,  
X<sub>1</sub> X<sub>2</sub> = [  
&nbsp; &nbsp;  _x_<sup>(1)</sup><sub>00</sub>  _x_<sup>(2)</sup><sub>00</sub> +
_x_<sup>(1)</sup><sub>01</sub>  _x_<sup>(2)</sup><sub>10</sub> ,
&nbsp; &nbsp;  _x_<sup>(1)</sup><sub>00</sub>  _x_<sup>(2)</sup><sub>01</sub> +
_x_<sup>(1)</sup><sub>01</sub>  _x_<sup>(2)</sup><sub>11</sub> ;  
&nbsp; &nbsp;  _x_<sup>(1)</sup><sub>10</sub>  _x_<sup>(2)</sup><sub>00</sub> +
_x_<sup>(1)</sup><sub>11</sub>  _x_<sup>(2)</sup><sub>10</sub> ,
&nbsp; &nbsp;  _x_<sup>(1)</sup><sub>10</sub>  _x_<sup>(2)</sup><sub>01</sub> +
_x_<sup>(1)</sup><sub>11</sub>  _x_<sup>(2)</sup><sub>11</sub>  
] ,  
X<sub>2</sub> X<sub>0</sub> = [  
&nbsp; &nbsp;  _x_<sup>(2)</sup><sub>00</sub>  _x_<sup>(0)</sup><sub>00</sub> +
_x_<sup>(2)</sup><sub>01</sub>  _x_<sup>(0)</sup><sub>10</sub> ,
&nbsp; &nbsp;  _x_<sup>(2)</sup><sub>00</sub>  _x_<sup>(0)</sup><sub>01</sub> +
_x_<sup>(2)</sup><sub>01</sub>  _x_<sup>(0)</sup><sub>11</sub> ;  
&nbsp; &nbsp;  _x_<sup>(2)</sup><sub>10</sub>  _x_<sup>(0)</sup><sub>00</sub> +
_x_<sup>(2)</sup><sub>11</sub>  _x_<sup>(0)</sup><sub>10</sub> ,
&nbsp; &nbsp;  _x_<sup>(2)</sup><sub>10</sub>  _x_<sup>(0)</sup><sub>01</sub> +
_x_<sup>(2)</sup><sub>11</sub>  _x_<sup>(0)</sup><sub>11</sub>  
] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(0)</sup><sub>00</sub> =
[ 1 , 0 ; 0 , 0 ] X<sub>1</sub> = [ _x_<sup>(1)</sup><sub>00</sub> , _x_<sup>(1)</sup><sub>01</sub> ; 0 , 0 ] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(0)</sup><sub>01</sub> =
[ 0 , 1 ; 0 , 0 ] X<sub>1</sub> = [ _x_<sup>(1)</sup><sub>10</sub> , _x_<sup>(1)</sup><sub>11</sub> ; 0 , 0 ] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(0)</sup><sub>10</sub> =
[ 0 , 0 ; 1 , 0 ] X<sub>1</sub> = [ 0 , 0 ; _x_<sup>(1)</sup><sub>00</sub> , _x_<sup>(1)</sup><sub>01</sub> ] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(0)</sup><sub>11</sub> =
[ 0 , 0 ; 0 , 1 ] X<sub>1</sub> = [ 0 , 0 ; _x_<sup>(1)</sup><sub>10</sub> , _x_<sup>(1)</sup><sub>11</sub> ] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(1)</sup><sub>00</sub> = X<sub>0</sub> [ 1 , 0 ; 0 , 0 ] =
[ _x_<sup>(0)</sup><sub>00</sub> , 0 ; _x_<sup>(0)</sup><sub>10</sub> , 0 ] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(1)</sup><sub>01</sub> = X<sub>0</sub> [ 0 , 1 ; 0 , 0 ] =
[ 0 , _x_<sup>(0)</sup><sub>00</sub> ; 0 , _x_<sup>(0)</sup><sub>10</sub> ] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(1)</sup><sub>10</sub> = X<sub>0</sub> [ 0 , 0 ; 1 , 0 ] =
[ _x_<sup>(0)</sup><sub>01</sub> , 0 ; _x_<sup>(0)</sup><sub>11</sub> , 0 ] ,  
_d_ (X<sub>0</sub> X<sub>1</sub>)/d _x_<sup>(1)</sup><sub>11</sub> = X<sub>0</sub> [ 0 , 0 ; 0 , 1 ] =
[ 0 , _x_<sup>(0)</sup><sub>01</sub> ; 0 , _x_<sup>(0)</sup><sub>11</sub> ] ,  

_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(1)</sup><sub>00</sub> =
[ 1 , 0 ; 0 , 0 ] X<sub>2</sub> = [ _x_<sup>(2)</sup><sub>00</sub> , _x_<sup>(2)</sup><sub>01</sub> ; 0 , 0 ] ,  
_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(1)</sup><sub>01</sub> =
[ 0 , 1 ; 0 , 0 ] X<sub>2</sub> = [ _x_<sup>(2)</sup><sub>10</sub> , _x_<sup>(2)</sup><sub>11</sub> ; 0 , 0 ] ,  
_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(1)</sup><sub>10</sub> =
[ 0 , 0 ; 1 , 0 ] X<sub>2</sub> = [ 0 , 0 ; _x_<sup>(2)</sup><sub>00</sub> , _x_<sup>(2)</sup><sub>01</sub> ] ,  
_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(1)</sup><sub>11</sub> =
[ 0 , 0 ; 0 , 1 ] X<sub>2</sub> = [ 0 , 0 ; _x_<sup>(2)</sup><sub>10</sub> , _x_<sup>(2)</sup><sub>11</sub> ] ,  
_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(2)</sup><sub>00</sub> = X<sub>1</sub> [ 1 , 0 ; 0 , 0 ] =
[ _x_<sup>(1)</sup><sub>00</sub> , 0 ; _x_<sup>(1)</sup><sub>10</sub> , 0 ] ,  
_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(2)</sup><sub>01</sub> = X<sub>1</sub> [ 0 , 1 ; 0 , 0 ] =
[ 0 , _x_<sup>(1)</sup><sub>00</sub> ; 0 , _x_<sup>(1)</sup><sub>10</sub> ] ,  
_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(2)</sup><sub>10</sub> = X<sub>1</sub> [ 0 , 0 ; 1 , 0 ] =
[ _x_<sup>(1)</sup><sub>01</sub> , 0 ; _x_<sup>(1)</sup><sub>11</sub> , 0 ] ,  
_d_ (X<sub>1</sub> X<sub>2</sub>)/d _x_<sup>(2)</sup><sub>11</sub> = X<sub>1</sub> [ 0 , 0 ; 0 , 1 ] =
[ 0 , _x_<sup>(1)</sup><sub>01</sub> ; 0 , _x_<sup>(1)</sup><sub>11</sub> ] ,  

_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(0)</sup><sub>00</sub> = X<sub>2</sub> [ 1 , 0 ; 0 , 0 ] =
[ _x_<sup>(2)</sup><sub>00</sub> , 0 ; _x_<sup>(2)</sup><sub>10</sub> , 0 ] ,  
_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(0)</sup><sub>01</sub> = X<sub>2</sub> [ 0 , 1 ; 0 , 0 ] =
[ 0 , _x_<sup>(2)</sup><sub>00</sub> ; 0 , _x_<sup>(2)</sup><sub>10</sub> ] ,  
_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(0)</sup><sub>10</sub> = X<sub>2</sub> [ 0 , 0 ; 1 , 0 ] =
[ _x_<sup>(2)</sup><sub>01</sub> , 0 ; _x_<sup>(2)</sup><sub>11</sub> , 0 ] ,  
_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(0)</sup><sub>11</sub> = X<sub>2</sub> [ 0 , 0 ; 0 , 1 ] =
[ 0 , _x_<sup>(2)</sup><sub>01</sub> ; 0 , _x_<sup>(2)</sup><sub>11</sub> ] ,  
_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(2)</sup><sub>00</sub> =
[ 1 , 0 ; 0 , 0 ] X<sub>0</sub> = [ _x_<sup>(0)</sup><sub>00</sub> , _x_<sup>(0)</sup><sub>01</sub> ; 0 , 0 ] ,  
_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(2)</sup><sub>01</sub> =
[ 0 , 1 ; 0 , 0 ] X<sub>0</sub> = [ _x_<sup>(0)</sup><sub>10</sub> , _x_<sup>(0)</sup><sub>11</sub> ; 0 , 0 ] ,  
_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(2)</sup><sub>10</sub> =
[ 0 , 0 ; 1 , 0 ] X<sub>0</sub> = [ 0 , 0 ; _x_<sup>(0)</sup><sub>00</sub> , _x_<sup>(0)</sup><sub>01</sub> ] ,  
_d_ (X<sub>2</sub> X<sub>0</sub>)/d _x_<sup>(2)</sup><sub>11</sub> =
[ 0 , 0 ; 0 , 1 ] X<sub>0</sub> = [ 0 , 0 ; _x_<sup>(0)</sup><sub>10</sub> , _x_<sup>(0)</sup><sub>11</sub> ] ,  

J = [  
&nbsp; &nbsp; _x_<sup>(1)</sup><sub>00</sub> , _x_<sup>(1)</sup><sub>10</sub> , 0 , 0 , _x_<sup>(0)</sup><sub>00</sub> , 0 , _x_<sup>(0)</sup><sub>01</sub> , 0 , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; _x_<sup>(1)</sup><sub>01</sub> , _x_<sup>(1)</sup><sub>11</sub> , 0 , 0 , 0 , _x_<sup>(0)</sup><sub>00</sub> , 0 , _x_<sup>(0)</sup><sub>01</sub> , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , _x_<sup>(1)</sup><sub>00</sub> , _x_<sup>(1)</sup><sub>10</sub> , _x_<sup>(0)</sup><sub>10</sub> , 0 , _x_<sup>(0)</sup><sub>11</sub> , 0 , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , _x_<sup>(1)</sup><sub>01</sub> , _x_<sup>(1)</sup><sub>11</sub> , 0 , _x_<sup>(0)</sup><sub>10</sub> , 0 , _x_<sup>(0)</sup><sub>11</sub> , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , _x_<sup>(2)</sup><sub>00</sub> , _x_<sup>(2)</sup><sub>10</sub> , 0 , 0 , _x_<sup>(1)</sup><sub>00</sub> , 0 , _x_<sup>(1)</sup><sub>01</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , _x_<sup>(2)</sup><sub>01</sub> , _x_<sup>(2)</sup><sub>11</sub> , 0 , 0 , 0 , _x_<sup>(1)</sup><sub>00</sub> , 0 , _x_<sup>(1)</sup><sub>01</sub> ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , 0 , 0 , _x_<sup>(2)</sup><sub>00</sub> , _x_<sup>(2)</sup><sub>10</sub> , _x_<sup>(1)</sup><sub>10</sub> , 0 , _x_<sup>(1)</sup><sub>11</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , 0 , 0 , _x_<sup>(2)</sup><sub>01</sub> , _x_<sup>(2)</sup><sub>11</sub> , 0 , _x_<sup>(1)</sup><sub>10</sub> , 0 , _x_<sup>(1)</sup><sub>11</sub> ;  
&nbsp; &nbsp; _x_<sup>(2)</sup><sub>00</sub> , 0 , _x_<sup>(2)</sup><sub>01</sub> , 0 , 0 , 0 , 0 , 0 , _x_<sup>(0)</sup><sub>00</sub> , _x_<sup>(0)</sup><sub>10</sub> , 0 , 0 ;  
&nbsp; &nbsp; 0 , _x_<sup>(2)</sup><sub>00</sub> , 0 , _x_<sup>(2)</sup><sub>01</sub> , 0 , 0 , 0 , 0 , _x_<sup>(0)</sup><sub>01</sub> , _x_<sup>(0)</sup><sub>11</sub> , 0 , 0 ;  
&nbsp; &nbsp; _x_<sup>(2)</sup><sub>10</sub> , 0 , _x_<sup>(2)</sup><sub>11</sub> , 0 , 0 , 0 , 0 , 0 , 0 , 0 , _x_<sup>(0)</sup><sub>00</sub> , _x_<sup>(0)</sup><sub>10</sub> ;  
&nbsp; &nbsp; 0 , _x_<sup>(2)</sup><sub>10</sub> , 0 , _x_<sup>(2)</sup><sub>11</sub> , 0 , 0 , 0 , 0 , 0 , 0 ,
_x_<sup>(0)</sup><sub>01</sub> , _x_<sup>(0)</sup><sub>11</sub> 
]