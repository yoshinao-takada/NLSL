# Gauss-Newton method basics
## 1-D Newton method
The generic proglem is represented as  
$\begin{equation}
0=\text{f}(x)
\end{equation}$
Setting _x_ to its initial value _x_<sub>0</sub> in above equation,  
$\begin{equation}
\text{f}_0=\text{f}(x_0)
\end{equation}$
zero-cross point of the linear line passing through $(x_0,\text{f}_0)$ with the slope
${d\text{f}/dx}(x_0)$ satisfies  
$\begin{equation}
\text{f}(x_1)-\text{f}(x_0)=
(x_1-x_0){d\text{f}\over dx}(x_0)
\end{equation}$  
Setting $\text{f}(x_1)=0$,
$\begin{equation}
x_1=x_0-\text{f}(x_0)/{d\text{f}\over{dx}}(x_0)
\end{equation}$  
Generalizing the iteration,
$\begin{equation}
x_{i+1}=x_{i}-\text{f}(x_{i})/{d\text{f}\over{dx}}(x_{i})
\end{equation}$  

## _k_-dimensional variables, _k_-dimensional functions, simultaneous equation
$\bold{x}$ and $\bold{f}$ are _k_ dimensional vectors.  
$\begin{equation}
\bold{x}_{i+1}=\bold{x}_{i}-
\bold{J}_{\text{F}}^{-1}(\bold{x}_{i})\bold{F}(\bold{x}_{i})\text{,}
\end{equation}$  
where $\bold{J}_{\text{F}}^{-1}(\bold{x}_{i})$ is a Jacobian of $\bold{F}(\bold{x}_{i})$
## _k_ variables, _m_ functions (_m_ > _k_), simultaneous equation
$\bold{x}$ is _k_-dimensional vector. $\bold{f}$ is an _m_-dimensional vector function of $\bold{x}$.
In this case, (3) in 1-dimensional case is written as  
$\begin{equation}
\bold{f}(\bold{x}_1)-\bold{f}(\bold{x}_0)={\bold{J}_F}(\bold{x}_1-\bold{x}_0)
\end{equation}$
It is an overly determined simultaneous equation. Applying LMS(Least Mean Square) technique,  
$\begin{equation}
\bold{J}_F^{T}\{\bold{f}(\bold{x}_1)-\bold{f}(\bold{x}_0)\}=
\bold{J}_F^{T}{\bold{J}_F}(\bold{x}_1-\bold{x}_0)
\end{equation}$
Setting $\bold{f}(\bold{x}_1)=0$,
$\begin{equation}
-\bold{J}_F^{T}\bold{f}(\bold{x}_0)=
\bold{J}_F^{T}{\bold{J}_F}(\bold{x}_1-\bold{x}_0)\text{,}
\end{equation}$
$\begin{equation}
\bold{x}_1 = \bold{x}_0-\{\bold{J}_F^{T}{\bold{J}_F}\}^{-1}\bold{J}_F^{T}\bold{f}(\bold{x}_0)
\end{equation}$
Generalizing the iteration, $\bold{f}(\bold{x}_i)$ approaches $\bold{0}$
$\begin{equation}
\bold{x}_{i+1} = \bold{x}_i-\{\bold{J}_F^{T}{\bold{J}_F}\}^{-1}\bold{J}_F^{T}\bold{f}(\bold{x}_i)
\end{equation}$

# Practice
## Practice 1: Calculate Polar Coordinate from Cartesian Coordinate
### **Formulation**
A polar coordinate is represented by  
$r$ : a distance from the origin,  
$\theta$ : elevation angle from Z-plane,  
$\phi$ : azimuthal angle from X-axis,  
and the relationship between the polar coordinate and the cartesian coordinate is
$\begin{equation}\begin{matrix}
x & = & r\cos(\theta)\cos(\phi) & = & r c_\theta c_\phi \\
y & = & r\cos(\theta)\sin(\phi) & = & r c_\theta s_\phi \\
z & = & r\sin(\theta) & = & r s_\theta\\
& & 1 &  = & {c_\theta}^2+{s_\theta}^2 \\
& & 1 & = & {c_\phi}^2+{s_\phi}^2
\end{matrix}\end{equation}$
Getting the polar coordinate from the cartesian coordinate using Gauss-Newton method,
the objective function is

$\begin{equation}\bold{F}(r,c_\theta, s_\theta, c_\phi, s_\phi) = \begin{bmatrix}
r c_\theta c_\phi - x \\
r c_\theta s_\phi - y \\
r s_\theta - z \\
{c_\theta}^2+{s_\theta}^2-1 \\
{c_\phi}^2+{s_\phi}^2-1 \\
\end{bmatrix} = \bold{0}
\end{equation}$
Jacobian $\bold{J}$ is

$\begin{equation}
\bold{J}=\begin{bmatrix}\frac{\partial\bold{F}}{\partial r} && \frac{\partial\bold{F}}{\partial c_\theta} && \frac{\partial\bold{F}}{\partial s_\theta} && \frac{\partial\bold{F}}{\partial c_\phi} && \frac{\partial\bold{F}}{\partial s_\phi} \end{bmatrix}\end{equation}$

$\begin{equation}
\frac{\partial\bold{F}}{\partial r} = \begin{bmatrix}
c_\theta c_\phi &
c_\theta s_\phi &
s_\theta & 0 & 0
\end{bmatrix}^t\end{equation}$

$\begin{equation}\frac{\partial\bold{F}}{\partial c_\theta} = \begin{bmatrix}r c_\phi & r s_\phi & 0 & 0 & 0
\end{bmatrix}^t\end{equation}$

$\begin{equation}\frac{\partial\bold{F}}{\partial s_\theta} = \begin{bmatrix}0 & 0 & r & 2 s_\theta & 0\end{bmatrix}^t\end{equation}$

$\begin{equation}
\frac{\partial\bold{F}}{\partial c_\phi} = \begin{bmatrix}
r c_\theta & 0 & 0 & 0 & 2c_\phi
\end{bmatrix}^t
\end{equation}$

$\begin{equation}
\frac{\partial\bold{F}}{\partial s_\phi} = \begin{bmatrix}
0 & r c_\theta & 0 & 0 & 2 s_\phi
\end{bmatrix}^t
\end{equation}$

### **Implementation**
The practice is implemented in `NLSLapp4.c`. Six callback functions which types are `NLSL_OBJECTIVE` are implemented.

Table 1: Callback functions
Name | Description
-----|-------------
objective | The objective function (refer to (13))
j0 | Jacobian column 0 (refer to (15))
j1 | Jacobian column 1 (refer to (15))
j2 | Jacobian column 2 (refer to (15))
j3 | Jacobian column 3 (refer to (15))
j4 | Jacobian column 4 (refer to (15))
<br/>

Variables $\bold{x}$ corresponds to components of polar coordinate as shown below.
Table 2: Polar coordinate variables
$\bold{x}$[*] | components of polar coordinate
-----|-------------
$\bold{x}$[0] | $r$
$\bold{x}$[1] | $c_\theta$
$\bold{x}$[2] | $s_\theta$
$\bold{x}$[3] | $c_\phi$
$\bold{x}$[4] | $s_\phi$
<br/>

Cartesian coordinate components, $x$, $y$, $z$ are given in `params` which is the last argument of
the callback functions.


