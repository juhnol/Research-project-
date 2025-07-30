
# Flow in Gaussian Random Fields
**Author: John McCance**  
**University of Southern California**

---

## Overview

This project simulates the transport of passive solutes in two-dimensional incompressible Gaussian random velocity fields, as described in Dentz et al., *Phys. Rev. E*, 2003. It numerically solves the Langevin and Fokker–Planck equations to analyze solute dispersion, using both Euler-Maruyama and extended Runge-Kutta integration. The focus is on capturing critical features like closed streamlines, anisotropic dispersion, and trapping behavior.

---

## Hypothesis

We hypothesize that using a higher-order numerical integration method, such as the extended Runge-Kutta scheme, will yield significantly more accurate representations of particle trajectories in 2D Gaussian random velocity fields compared to simpler methods like Euler-Maruyama. Specifically:

1. **Streamline fidelity**: Runge-Kutta will better capture the formation of closed streamlines
2. **Dispersion accuracy**: The method will produce dispersion coefficients (especially longitudinal growth) that more closely align with theoretical predictions.
3. **Reduced numerical artifacts**: Runge-Kutta will suppress artificial transverse dispersion and noise-induced diffusion, particularly in high Péclet number regimes.

The aim is to test whether the improved integration leads to observable differences in both trajectory structure and long-time dispersion scaling.

## Governing Equations

### 1. Fokker–Planck Equation (macroscopic concentration evolution)

```math
\frac{\partial C}{\partial t} + \nabla \cdot (\vec{u} C - D \nabla C) = 0
```

Where:

- `C(\vec{x}, t)`: Solute concentration  
- `\vec{u}(\vec{x})`: Velocity field  
- `D`: Local diffusion coefficient

### 2. Langevin Equation (single-particle trajectory)

```math
\frac{d\vec{X}(t)}{dt} = \vec{u}(\vec{X}(t)) + \vec{\xi}(t)
```

with:

```math
\langle \xi_i(t) \xi_j(t') \rangle = 2D \, \delta_{ij} \, \delta(t - t')
```

---

## Numerical Methods

### Velocity field (spectral method):

```math
u_i(\vec{x}) = \bar{u} \, \delta_{i1} + 2 \sigma \bar{u} \sum_{j=1}^{N} p_i(\vec{k}^{(j)}) \cos(\vec{k}^{(j)} \cdot \vec{x} + \omega^{(j)})
```

where:

```math
p_i(\vec{k}) = \delta_{1i} - \frac{k_1 k_i}{|\vec{k}|^2}
```

ensures incompressibility.

### Particle simulation methods:

- Euler-Maruyama method (1st-order accuracy)
- Extended Runge-Kutta (order `\Delta t^{3/2}`): More accurate in high Péclet number (`\text{Pe} = \frac{\bar{u} \ell}{D} \gg 1`) regimes

### Dispersion coefficients:

**Effective:**

```math
D_{ij}^{\text{eff}}(t) = \frac{1}{2} \frac{d}{dt} \left[ \langle x_i(t) x_j(t) \rangle - \langle x_i(t) \rangle \langle x_j(t) \rangle \right]
```

**Ensemble:**

```math
D_{ij}^{\text{ens}}(t) = \frac{1}{2} \frac{d}{dt} \left[ \overline{\langle x_i(t) x_j(t) \rangle} - \overline{\langle x_i(t) \rangle} \, \overline{\langle x_j(t) \rangle} \right]
```

---

## Key Phenomena

### Closed Streamlines:

```math
\vec{u} = \nabla^\perp \psi(\vec{x}) \Rightarrow \text{Streamlines: } \psi = \text{const}
```

Leads to subdiffusion and localized zones without dispersion unless noise breaks symmetry.

### Linear Growth of Longitudinal Dispersion:

```math
D_L^{\text{ens}}(t) \sim r(1 - r) u^2 t
```

(See Appendix D of Dentz et al., 2003)

---

## Experimental Setup

- Velocity field modes: `N = 20–64` harmonic modes  
- Particles: Gaussian initial distribution  
- Boundary conditions: Infinite domain with decay at infinity  
- Metrics: Trajectories, streamline plots, longitudinal/transverse dispersion

---

## Physical Constraints

### Incompressibility:

```math
\nabla \cdot \vec{u} = 0
```

Enforced through projection:

```math
p_i(\vec{k}) = \delta_{1i} - \frac{k_1 k_i}{|\vec{k}|^2}
```

Each harmonic mode satisfies incompressibility individually.

### Mass Conservation:

```math
\int C(\vec{x}, t) \, d\vec{x} = \text{constant}
```

### Velocity Correlation / Energy Consistency:

```math
\langle \tilde{u}_i(\vec{k}) \tilde{u}_j(\vec{k}') \rangle = (2\pi)^2 \delta(\vec{k} + \vec{k}') \, p_i(\vec{k}) p_j(\vec{k}) \, C_{ff}(\vec{k})
```

with:

```math
C_{ff}(\vec{k}) = \frac{\sigma^2}{2\pi \ell^2} \exp\left( -\frac{|\vec{k}|^2 \ell^2}{2} \right)
```

---

## Dispersion Scaling

- Longitudinal dispersion: `D_L^{\text{ens}}(t) \sim t` (linear growth)  
- Transverse dispersion: `D_T^{\text{ens}}(t) \rightarrow \text{const}` (saturation)

---

## Summary of Results

| Method         | Behavior Captured                                     | Notes                                      |
|----------------|--------------------------------------------------------|--------------------------------------------|
| Euler-Maruyama | Isotropic diffusion only                              | Misses streamlines, overestimates spread   |
| Runge-Kutta    | Anisotropic dispersion, streamline trapping, DL growth | Matches Dentz et al. 2003 findings         |




