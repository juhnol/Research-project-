# Research-project-

Flow in Gaussian Random Fields

Overview
This project simulates the transport of passive solutes in two-dimensional incompressible Gaussian random velocity fields, as described in Dentz et al., Phys. Rev. E, 2003. It numerically solves the Langevin and Fokker–Planck equations to analyze solute dispersion, using both Euler-Maruyama and extended Runge-Kutta integration. The focus is on capturing critical features like closed streamlines, anisotropic dispersion, and trapping behavior.


Governing Equations
1. Fokker–Planck Equation (macroscopic concentration evolution)

∂
C
∂
t
+
∇
⋅
(
u
⃗
C
−
D
∇
C
)
=
0
∂t
∂C
​	
 +∇⋅( 
u
 C−D∇C)=0
Where:

C
(
x
⃗
,
t
)
C( 
x
 ,t): Solute concentration
u
⃗
(
x
⃗
)
u
 ( 
x
 ): Velocity field
D
D: Local diffusion coefficient
2. Langevin Equation (single-particle trajectory)

d
X
⃗
(
t
)
d
t
=
u
⃗
(
X
⃗
(
t
)
)
+
ξ
⃗
(
t
)
dt
d 
X
 (t)
​	
 = 
u
 ( 
X
 (t))+ 
ξ
​	
 (t)
with 
⟨
ξ
i
(
t
)
ξ
j
(
t
′
)
⟩
=
2
D
δ
i
j
δ
(
t
−
t
′
)
with ⟨ξ 
i
​	
 (t)ξ 
j
​	
 (t 
′
 )⟩=2Dδ 
ij
​	
 δ(t−t 
′
 )
🔬 Numerical Methods
Velocity field:
Spectral method using:
u
i
(
x
⃗
)
=
u
ˉ
δ
i
1
+
2
σ
u
ˉ
2
N
∑
j
=
1
N
p
i
(
k
⃗
(
j
)
)
cos
⁡
(
k
⃗
(
j
)
⋅
x
⃗
+
ω
(
j
)
)
u 
i
​	
 ( 
x
 )= 
u
ˉ
 δ 
i1
​	
 +2σ 
u
ˉ
  
N
2
​	
 
​	
  
j=1
∑
N
​	
 p 
i
​	
 ( 
k
  
(j)
 )cos( 
k
  
(j)
 ⋅ 
x
 +ω 
(j)
 )
where 
p
i
(
k
⃗
)
=
δ
1
i
−
k
1
k
i
∣
k
⃗
∣
2
p 
i
​	
 ( 
k
 )=δ 
1i
​	
 − 
∣ 
k
 ∣ 
2
 
k 
1
​	
 k 
i
​	
 
​	
  ensures incompressibility.
Particle simulation:
Euler-Maruyama method (1st-order accuracy)
Extended Runge-Kutta (order 
Δ
t
3
/
2
Δt 
3/2
 ):
More accurate in high Péclet number (
Pe
=
u
ˉ
ℓ
D
≫
1
Pe= 
D
u
ˉ
 ℓ
​	
 ≫1) regimes
Dispersion coefficients:
Effective:
D
i
j
eff
(
t
)
=
1
2
d
d
t
[
⟨
x
i
(
t
)
x
j
(
t
)
⟩
−
⟨
x
i
(
t
)
⟩
⟨
x
j
(
t
)
⟩
]
D 
ij
eff
​	
 (t)= 
2
1
​	
  
dt
d
​	
 [⟨x 
i
​	
 (t)x 
j
​	
 (t)⟩−⟨x 
i
​	
 (t)⟩⟨x 
j
​	
 (t)⟩]
Ensemble:
D
i
j
ens
(
t
)
=
1
2
d
d
t
[
⟨
x
i
(
t
)
x
j
(
t
)
⟩
‾
−
⟨
x
i
(
t
)
⟩
‾
 
⟨
x
j
(
t
)
⟩
‾
]
D 
ij
ens
​	
 (t)= 
2
1
​	
  
dt
d
​	
 [ 
⟨x 
i
​	
 (t)x 
j
​	
 (t)⟩
​	
 − 
⟨x 
i
​	
 (t)⟩
​	
  
⟨x 
j
​	
 (t)⟩
​	
 ]


 Key Phenomena
Closed Streamlines:
In 2D Gaussian fields, particles can become trapped:
u
⃗
=
∇
⊥
ψ
(
x
⃗
)
⇒
Streamlines 
ψ
=
const
u
 =∇ 
⊥
 ψ( 
x
 )⇒Streamlines ψ=const
Leads to subdiffusion and localized zones without dispersion unless noise breaks symmetry.
Linear Growth of Longitudinal Dispersion:
Even in the absence of local diffusion:
D
L
ens
(
t
)
∼
r
(
1
−
r
)
u
2
t
D 
L
ens
​	
 (t)∼r(1−r)u 
2
 t
(See Appendix D of Dentz et al., 2003)

Experimental Setup
Velocity field modes: N = 20–64 harmonic modes
Particles: Gaussian initial distribution
Boundary conditions: Infinite domain with decay at infinity
Metrics: Trajectories, streamline plots, longitudinal/transverse dispersion



Physical Constraints
To ensure realistic simulation of solute transport in incompressible flow fields, the following physical constraints are enforced:

1. Incompressibility

The velocity field 
u
⃗
(
x
⃗
)
u
 ( 
x
 ) must be divergence-free:

∇
⋅
u
⃗
=
0
∇⋅ 
u
 =0
This condition ensures volume conservation and is enforced through the spectral method using projection:

p
i
(
k
⃗
)
=
δ
1
i
−
k
1
k
i
∣
k
⃗
∣
2
p 
i
​	
 ( 
k
 )=δ 
1i
​	
 − 
∣ 
k
 ∣ 
2
 
k 
1
​	
 k 
i
​	
 
​	
 
Each harmonic mode satisfies incompressibility individually.


2. Mass Conservation

The total mass of the solute must be conserved over time:

∫
C
(
x
⃗
,
t
)
 
d
x
⃗
=
constant
∫C( 
x
 ,t)d 
x
 =constant
Simulations verify this by checking conservation across timesteps, especially in low-diffusion regimes where numerical error could accumulate.

3. Energy Consistency / Velocity Correlation

The velocity field must obey the theoretical autocorrelation:

⟨
u
~
i
(
k
⃗
)
u
~
j
(
k
′
⃗
)
⟩
=
(
2
π
)
2
δ
(
k
⃗
+
k
′
⃗
)
p
i
(
k
⃗
)
p
j
(
k
⃗
)
C
f
f
(
k
⃗
)
⟨ 
u
~
  
i
​	
 ( 
k
 ) 
u
~
  
j
​	
 ( 
k 
′
 
 )⟩=(2π) 
2
 δ( 
k
 + 
k 
′
 
 )p 
i
​	
 ( 
k
 )p 
j
​	
 ( 
k
 )C 
ff
​	
 ( 
k
 )
With:

C
f
f
(
k
⃗
)
=
σ
2
(
2
π
ℓ
2
)
exp
⁡
(
−
∣
k
⃗
∣
2
ℓ
2
2
)
C 
ff
​	
 ( 
k
 )=σ 
2
 (2πℓ 
2
 )exp(− 
2
∣ 
k
 ∣ 
2
 ℓ 
2
 
​	
 )
This ensures the generated field maintains correct variance 
σ
2
σ 
2
  and correlation length 
ℓ
ℓ, as required by the Gaussian assumption.

Dispersion Scaling

Correct long-time behavior must emerge:

Longitudinal dispersion:
D
L
ens
(
t
)
∼
t
(linear growth)
D 
L
ens
​	
 (t)∼t(linear growth)
Transverse dispersion:
D
T
ens
(
t
)
→
const
(saturation)
D 
T
ens
​	
 (t)→const(saturation)
These scaling laws validate that both advection and diffusion are resolved appropriately.


Summary of Results:


| Method         | Behavior Captured                                      | Notes                                    |
| -------------- | ------------------------------------------------------ | ---------------------------------------- |
| Euler-Maruyama | Isotropic diffusion only                               | Misses streamlines, overestimates spread |
| Runge-Kutta    | Anisotropic dispersion, streamline trapping, DL growth | Matches Dentz et al. 2003 findings       |



Author
John McCance
University of Southern California




