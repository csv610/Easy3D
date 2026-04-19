# Surface Curvatures: A Study Note

In differential geometry and 3D mesh processing, curvature describes how much a surface deviates from being flat at a given point. For a surface mesh, these values are typically computed per vertex using local neighborhood analysis.

The `SurfaceMeshCurvature` class in Easy3D implements the calculation of the following fundamental curvatures:

## 1. Principal Curvatures ($\kappa_{min}$, $\kappa_{max}$)

At any point on a smooth surface, there are two perpendicular directions (the principal directions) where the normal curvature of the surface reaches its maximum and minimum values. These values are the principal curvatures:

*   **$\kappa_{max}$ (Maximum Curvature):** The largest eigenvalue of the shape operator.
*   **$\kappa_{min}$ (Minimum Curvature):** The smallest eigenvalue of the shape operator.

In Easy3D, these are stored as vertex properties `"v:curv-min"` and `"v:curv-max"`.

## 2. Gaussian Curvature ($K$)

Gaussian curvature is an **intrinsic** measure of curvature, meaning it can be determined by measuring distances along the surface and does not change even if the surface is bent without stretching (isometric deformation).

$$K = \kappa_{min} \cdot \kappa_{max}$$

### Geometric Interpretation:
*   **$K > 0$ (Elliptic):** The surface is bowl-like (locally on one side of the tangent plane). Points like the peak of a mountain.
*   **$K < 0$ (Hyperbolic):** The surface is saddle-shaped. Principal curvatures have opposite signs.
*   **$K = 0$ (Parabolic/Flat):** At least one principal curvature is zero (e.g., a cylinder or a plane).

## 3. Mean Curvature ($H$)

Mean curvature is an **extrinsic** measure that describes the average of the principal curvatures. It is closely related to the surface area minimization and the concept of minimal surfaces.

$$H = \frac{\kappa_{min} + \kappa_{max}}{2}$$

### Geometric Interpretation:
*   **$H = 0$:** A **minimal surface** (like a soap film stretched across a wire frame).
*   The sign of mean curvature depends on the choice of surface normal orientation.

## 4. Maximum Absolute Curvature ($\kappa_{abs}$)

This represents the largest magnitude of bending at a point, regardless of direction.

$$\kappa_{abs} = \max(|\kappa_{min}|, |\kappa_{max}|)$$

This is often used in feature detection to identify sharp edges or highly curved regions of a mesh.

---

## Implementation Details in Easy3D

Easy3D uses discrete differential geometry operators to estimate these values on triangular meshes. The implementation is primarily based on:
1.  **Discrete Laplace-Beltrami Operator** (Meyer et al. 2003): Using the "cotangent weight" formulation to estimate the mean curvature vector.
2.  **Angle Deficit Method:** Commonly used for Gaussian curvature, where the curvature at a vertex is $2\pi$ minus the sum of the angles of incident faces.
3.  **Curvature Tensor Estimation:** A more robust method (via `analyze_tensor`) that fits a local quadratic surface or uses the normal cycle theory.
