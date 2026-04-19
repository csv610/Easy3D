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
*   **$K = 0$ (Parabolic/Flat):** At least one principal curvature is zero (for example, in the case of a cylinder or a plane).

## 3. Mean Curvature ($H$)

Mean curvature is an **extrinsic** measure that describes the average of the principal curvatures. It is closely related to the surface area minimization and the concept of minimal surfaces.

$$H = \frac{\kappa_{min} + \kappa_{max}}{2}$$

### Geometric Interpretation:
*   **$H = 0$:** A **minimal surface** (such as a soap film stretched across a wire frame).
*   The sign of mean curvature depends on the choice of surface normal orientation.

## 4. Maximum Absolute Curvature ($\kappa_{abs}$)

This represents the largest magnitude of bending at a point, regardless of direction.

$$\kappa_{abs} = \max(|\kappa_{min}|, |\kappa_{max}|)$$

This is often used in feature detection to identify sharp edges or highly curved regions of a mesh.

---

## Normal Cycle Theory for Curvature Estimation

Normal Cycle Theory provides a robust mathematical framework for estimating curvature on non-smooth geometric objects, such as triangular meshes. This theory, pioneered by **Cohen-Steiner and Morvan (2003)**, allows the definition of curvature measures for any "geometric" set, including those with sharp edges and corners.

### The Core Concept: The Normal Cycle

In smooth differential geometry, curvature is a point-wise property. However, on a mesh, the surface is piecewise flat, and curvature is concentrated at edges and vertices. 

The **Normal Cycle** ($\mathbf{C}$) is a 2-cycle in the unit tangent bundle of the surface. Instead of looking only at the surface itself, the theory looks at the set of pairs $(p, \vec{n})$, where $p$ is a point on the surface and $\vec{n}$ is a unit normal vector at that point.

### Discrete Curvature Tensor Formulation

For a triangular mesh, the curvature tensor at a vertex $v$ is estimated by averaging the contributions of all edges $e$ in a local neighborhood $B$ (for example, a 1-ring or 2-ring neighborhood):

$$\mathbf{T}(v) = \frac{1}{|B|} \sum_{e \in B} \beta(e) \cdot L(e \cap B) \cdot \vec{e} \otimes \vec{e}$$

Where:
*   $|B|$: The area of the neighborhood (usually the Voronoi area).
*   $\beta(e)$: The **signed dihedral angle** at edge $e$ (positive for convex configurations, negative for concave configurations).
*   $L(e \cap B)$: The length of the part of edge $e$ contained within the neighborhood $B$.
*   $\vec{e}$: The unit vector in the direction of edge $e$.
*   $\vec{e} \otimes \vec{e}$: The outer product (tensor product), resulting in a $3 \times 3$ symmetric matrix.

---

## Winding Numbers and Winding Rules

The **Winding Number** is a fundamental concept in topology used to determine if a point lies inside or outside a closed curve (2D) or a closed surface (3D). It counts the number of times a curve travels counter-clockwise around a given point.

### 2D Winding Rules

In complex polygons with self-intersections or multiple nested contours, the winding number is used to define which regions are "inside" the polygon. The `Tessellator` class in Easy3D implements the following rules:

*   **WINDING_ODD (Default):** A point is inside if its winding number is odd. This is the standard rule for most simple polygons and creates "holes" in nested structures.
*   **WINDING_NONZERO:** A point is inside if its winding number is not zero. This is commonly used for union operations of multiple contours.
*   **WINDING_POSITIVE:** A point is inside if its winding number is greater than zero.
*   **WINDING_NEGATIVE:** A point is inside if its winding number is less than zero.
*   **WINDING_ABS_GEQ_TWO:** A point is inside if the absolute value of its winding number is 2 or greater (used for finding intersection regions).

### 3D Generalized Winding Number

While 2D winding numbers are straightforward, the **Generalized Winding Number** (Barill et al. 2018) extends this to 3D meshes that may not be perfectly watertight (i.e., containing holes, self-intersections, or degenerate geometry).

For a point $P$ and a triangular mesh $M$, the winding number $w(P)$ is defined by the solid angle integral:

$$w(P) = \sum_{f \in M} \frac{\Omega_f(P)}{4\pi}$$

Where $\Omega_f(P)$ is the signed solid angle of face $f$ subtended at point $P$. 
*   **$w(P) \approx 1$:** The point is inside the mesh.
*   **$w(P) \approx 0$:** The point is outside.
*   Values between 0 and 1 occur near boundaries or holes, providing a "fuzzy" or robust classification for imperfect meshes.

---

## Implementation Details in Easy3D

### Curvature Estimation
Easy3D uses discrete differential geometry operators to estimate these values on triangular meshes:
1.  **Discrete Laplace-Beltrami Operator** (Meyer et al. 2003): Using the "cotangent weight" formulation to estimate the mean curvature vector.
2.  **Angle Deficit Method:** Commonly used for Gaussian curvature, where the curvature at a vertex is $2\pi$ minus the sum of the angles of incident faces.
3.  **Curvature Tensor Estimation:** A more robust method (implemented via `analyze_tensor`) that uses the Normal Cycle Theory to compute the full curvature tensor.

### Tessellation and Winding
The `Tessellator` and `csg::tessellate` utilities use winding rules to handle complex polygon structures during mesh construction and rendering. This is particularly critical for font rendering (`TextMesher`) and polygon partitioning.

---
**References:** 
- *Cohen-Steiner, D., and Morvan, J-M. "Restricted Delaunay triangulations and normal cycle." 2003.*
- *Meyer, M. et al. "Discrete Differential-Geometry Operators for Triangulated 2-Manifolds." 2003.*
- *Barill, G. et al. "Fast Winding Numbers for Soups and Clouds." 2018.*
