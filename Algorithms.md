# Algorithms in Easy3D-M1

This document provides a comprehensive list of algorithms implemented and used in the Easy3D-M1 codebase, categorized by their primary data structures and functions.

## Point Cloud Processing

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Normal Estimation** | Estimates per-point normals using PCA-based local neighborhood analysis. Supports orientation consistency. | `PointCloudNormals` |
| **Poisson Surface Reconstruction** | Reconstructs a watertight surface mesh from an oriented point cloud using the screened Poisson method. | `PoissonReconstruction` |
| **RANSAC Primitive Extraction** | Robustly extracts geometric primitives (planes, spheres, cylinders, etc.) from point clouds. | `PrimitivesRansac` |
| **Point Cloud Simplification** | Reduces point cloud density while preserving important geometric features (e.g., via voxel grid downsampling). | `PointCloudSimplification` |

## Surface Mesh Algorithms

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Mesh Simplification** | Reduces the number of triangles in a mesh while preserving the overall shape (Quadric Error Metrics). | `SurfaceMeshSimplification` |
| **Mesh Smoothing** | Reduces noise in mesh geometry using Laplacian, Taubin, or bilateral filters. | `SurfaceMeshSmoothing` |
| **Incremental Remeshing** | Improves mesh quality by iteratively applying edge splits, collapses, swaps, and tangential smoothing. | `SurfaceMeshRemeshing` |
| **Hole Filling** | Automatically detects and fills holes in surface meshes with locally optimal triangulations. | `SurfaceMeshHoleFilling` |
| **Mesh Fairing** | Smooths mesh regions while satisfying boundary constraints (e.g., minimal surfaces). | `SurfaceMeshFairing` |
| **Mesh Triangulation** | Converts general polygonal faces into triangles. | `SurfaceMeshTriangulation` |
| **Mesh Subdivision** | Increases mesh resolution using schemes like Catmull-Clark or Loop subdivision. | `SurfaceMeshSubdivision` |
| **Mesh Parameterization** | Maps 3D mesh surfaces to a 2D domain (UV mapping) using harmonic or conformal mappings. | `SurfaceMeshParameterization` |
| **Mesh Sampling** | Generates uniform or importance-based point samples on the surface of a mesh. | `SurfaceMeshSampler` |
| **Curvature Estimation** | Computes Gaussian, mean, and principal curvatures at each vertex. | `SurfaceMeshCurvature` |
| **Mesh Stitching** | Combines multiple disjoint mesh components by stitching their boundaries. | `SurfaceMeshStitching` |
| **Geodesic Distance** | Computes shortest paths/distances on the mesh surface between vertices. | `SurfaceMeshGeodesic` |

## Polyhedral Mesh Algorithms

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Tetrahedralization** | Generates a 3D tetrahedral mesh from a closed surface mesh boundary (typically via TetGen). | `SurfaceMeshTetrahedralization` |

## General Geometry & Utilities

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Delaunay Triangulation** | Computes the Delaunay triangulation for 2D or 3D point sets. | `Delaunay2D`, `Delaunay3D` |
| **Polygon Partition** | Decomposes complex polygons into simpler convex pieces. | `PolygonPartition` |
| **Tessellation** | Robustly triangulates complex polygons with holes or self-intersections. | `Tessellator` |
| **Collision Detection** | Detects intersections and computes distances between geometric models. | `Collider` |
| **Spline Interpolation** | Cubic spline interpolation for curve fitting and data smoothing. | `SplineInterpolation` |
| **KD-Tree** | Efficient spatial indexing for fast nearest neighbor searches. | `KdTree`, `TriangleMeshKdTree` |

## CGAL-based Extensions (Optional)

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Self-Intersection Detection**| Detects faces that intersect with other faces in the same mesh. | `SelfIntersection` |
| **Overlapping Face Detection**| Identifies faces that are nearly coplanar and overlapping. | `OverlappingFaces` |
