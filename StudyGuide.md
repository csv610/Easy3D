# Normal Cycle Theory for Curvature Estimation

Normal Cycle Theory provides a robust mathematical framework for estimating curvature on non-smooth geometric objects, such as triangular meshes. This theory, pioneered by **Cohen-Steiner and Morvan (2003)**, allows the definition of curvature measures for any "geometric" set, including those with sharp edges and corners.

## The Core Concept: The Normal Cycle

In smooth differential geometry, curvature is a point-wise property. However, on a mesh, the surface is piecewise flat, and curvature is concentrated at edges and vertices. 

The **Normal Cycle** ($\mathbf{C}$) is a 2-cycle in the unit tangent bundle of the surface. Instead of looking only at the surface itself, the theory looks at the set of pairs $(p, \vec{n})$, where $p$ is a point on the surface and $\vec{n}$ is a unit normal vector at that point.

## Discrete Curvature Tensor Formulation

For a triangular mesh, the curvature tensor at a vertex $v$ is estimated by averaging the contributions of all edges $e$ in a local neighborhood $B$ (e.g., 1-ring or 2-ring):

$$\mathbf{T}(v) = \frac{1}{|B|} \sum_{e \in B} \beta(e) \cdot L(e \cap B) \cdot \vec{e} \otimes \vec{e}$$

Where:
*   $|B|$: The area of the neighborhood (usually the Voronoi area).
*   $\beta(e)$: The **signed dihedral angle** at edge $e$ (positive for convex, negative for concave).
*   $L(e \cap B)$: The length of the part of edge $e$ contained within the neighborhood $B$.
*   $\vec{e}$: The unit vector in the direction of edge $e$.
*   $\vec{e} \otimes \vec{e}$: The outer product (tensor product), resulting in a $3 \times 3$ symmetric matrix.

## Why it is Robust

1.  **Averaging:** Unlike the cotangent-weight method which can be sensitive to mesh quality, the Normal Cycle method integrates information over a local region, making it more stable against noise.
2.  **Tensor Representation:** It provides a full $3 \times 3$ matrix representing the local shape operator.
3.  **Convergence:** It is mathematically proven to converge to the true curvature tensor as the mesh resolution increases (under certain conditions).

## Implementation in Easy3D (`analyze_tensor`)

The function `SurfaceMeshCurvature::analyze_tensor` implements this theory:
1.  **Angle Calculation:** Computes the dihedral angle $\beta$ between adjacent faces for every edge.
2.  **Tensor Accumulation:** For each vertex, it sums the matrices $\beta \cdot L \cdot (\vec{e} \vec{e}^T)$ for all incident edges.
3.  **Eigen-Decomposition:** Performs eigen-decomposition on the resulting $3 \times 3$ matrix.
4.  **Extraction:**
    *   The eigenvector with the **smallest** absolute eigenvalue corresponds to the estimated **surface normal**.
    *   The other two eigenvectors are the **principal directions**.
    *   Their eigenvalues are the **principal curvatures** ($\kappa_{min}$, $\kappa_{max}$).

---
**Reference:** 
*Cohen-Steiner, David, and Jean-Marie Morvan. "Restricted Delaunay triangulations and normal cycle." Proceedings of the nineteenth annual symposium on Computational geometry. 2003.*
