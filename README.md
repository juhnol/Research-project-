\documentclass[12pt]{article}
\usepackage{amsmath, amssymb}
\usepackage{graphicx}
\usepackage{geometry}
\geometry{margin=1in}

\title{Flow in Gaussian Random Fields}
\author{John McCance \\ University of Southern California}
\date{}

\begin{document}

\maketitle

\section*{Overview}
This project simulates the transport of passive solutes in two-dimensional incompressible Gaussian random velocity fields, as described in Dentz et al., \textit{Phys. Rev. E}, 2003. It numerically solves the Langevin and Fokker–Planck equations to analyze solute dispersion, using both Euler-Maruyama and extended Runge-Kutta integration. The focus is on capturing critical features like closed streamlines, anisotropic dispersion, and trapping behavior.

\section*{Governing Equations}

\subsection*{1. Fokker–Planck Equation (macroscopic concentration evolution)}

\[
\frac{\partial C}{\partial t} + \nabla \cdot (\vec{u} C - D \nabla C) = 0
\]

Where:
\begin{itemize}
  \item $C(\vec{x}, t)$: Solute concentration
  \item $\vec{u}(\vec{x})$: Velocity field
  \item $D$: Local diffusion coefficient
\end{itemize}

\subsection*{2. Langevin Equation (single-particle trajectory)}

\[
\frac{d\vec{X}(t)}{dt} = \vec{u}(\vec{X}(t)) + \vec{\xi}(t)
\]

with:

\[
\langle \xi_i(t) \xi_j(t') \rangle = 2D \, \delta_{ij} \, \delta(t - t')
\]

\section*{Numerical Methods}

\textbf{Velocity field (spectral method):}

\[
u_i(\vec{x}) = \bar{u} \, \delta_{i1} + 2 \sigma \bar{u} \sum_{j=1}^{N} p_i(\vec{k}^{(j)}) \cos(\vec{k}^{(j)} \cdot \vec{x} + \omega^{(j)})
\]

where:

\[
p_i(\vec{k}) = \delta_{1i} - \frac{k_1 k_i}{|\vec{k}|^2}
\]

ensures incompressibility.

\textbf{Particle simulation:}
\begin{itemize}
  \item Euler-Maruyama method (1st-order accuracy)
  \item Extended Runge-Kutta (order $\Delta t^{3/2}$): More accurate in high Péclet number ($\text{Pe} = \frac{\bar{u} \ell}{D} \gg 1$) regimes
\end{itemize}

\textbf{Dispersion coefficients:}

Effective:

\[
D_{ij}^{\text{eff}}(t) = \frac{1}{2} \frac{d}{dt} \left[ \langle x_i(t) x_j(t) \rangle - \langle x_i(t) \rangle \langle x_j(t) \rangle \right]
\]

Ensemble:

\[
D_{ij}^{\text{ens}}(t) = \frac{1}{2} \frac{d}{dt} \left[ \overline{\langle x_i(t) x_j(t) \rangle} - \overline{\langle x_i(t) \rangle} \, \overline{\langle x_j(t) \rangle} \right]
\]

\section*{Key Phenomena}

\textbf{Closed Streamlines:}

\[
\vec{u} = \nabla^\perp \psi(\vec{x}) \Rightarrow \text{Streamlines: } \psi = \text{const}
\]

Leads to subdiffusion and localized zones without dispersion unless noise breaks symmetry.

\textbf{Linear Growth of Longitudinal Dispersion:}

\[
D_L^{\text{ens}}(t) \sim r(1 - r) u^2 t
\]

(See Appendix D of Dentz et al., 2003)

\section*{Experimental Setup}

\begin{itemize}
  \item Velocity field modes: $N = 20$–$64$ harmonic modes
  \item Particles: Gaussian initial distribution
  \item Boundary conditions: Infinite domain with decay at infinity
  \item Metrics: Trajectories, streamline plots, longitudinal/transverse dispersion
\end{itemize}

\section*{Physical Constraints}

\textbf{Incompressibility:}

\[
\nabla \cdot \vec{u} = 0
\]

Enforced through projection:

\[
p_i(\vec{k}) = \delta_{1i} - \frac{k_1 k_i}{|\vec{k}|^2}
\]

Each harmonic mode satisfies incompressibility individually.

\textbf{Mass Conservation:}

\[
\int C(\vec{x}, t) \, d\vec{x} = \text{constant}
\]

Simulations verify this by checking conservation across timesteps, especially in low-diffusion regimes where numerical error could accumulate.

\textbf{Velocity Correlation / Energy Consistency:}

\[
\langle \tilde{u}_i(\vec{k}) \tilde{u}_j(\vec{k}') \rangle = (2\pi)^2 \delta(\vec{k} + \vec{k}') \, p_i(\vec{k}) p_j(\vec{k}) \, C_{ff}(\vec{k})
\]

with:

\[
C_{ff}(\vec{k}) = \frac{\sigma^2}{2\pi \ell^2} \exp\left( -\frac{|\vec{k}|^2 \ell^2}{2} \right)
\]

\section*{Dispersion Scaling}

Correct long-time behavior must emerge:

\begin{itemize}
  \item Longitudinal dispersion: $D_L^{\text{ens}}(t) \sim t$ (linear growth)
  \item Transverse dispersion: $D_T^{\text{ens}}(t) \rightarrow \text{const}$ (saturation)
\end{itemize}

These scaling laws validate that both advection and diffusion are resolved appropriately.

\section*{Summary of Results}

\begin{center}
\begin{tabular}{|l|l|l|}
\hline
\textbf{Method} & \textbf{Behavior Captured} & \textbf{Notes} \\\hline
Euler-Maruyama & Isotropic diffusion only & Misses streamlines, overestimates spread \\\hline
Runge-Kutta & Anisotropic dispersion, streamline trapping, $D_L$ growth & Matches Dentz et al. 2003 findings \\\hline
\end{tabular}
\end{center}

\section*{Author}

John McCance \\
University of Southern California

\end{document}




