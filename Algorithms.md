# Algorithms in Easy3D-M1

This document provides a comprehensive list of algorithms implemented and used in the Easy3D-M1 codebase, categorized by their primary data structures and functions.

## Point Cloud Processing

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Normal Estimation** | Estimates per-point normals using PCA-based local neighborhood analysis and ensures consistent orientation across the point cloud. | `PointCloudNormals` |
| **Poisson Surface Reconstruction** | Reconstructs a watertight surface mesh from an oriented point cloud using the screened Poisson reconstruction method. | `PoissonReconstruction` |
| **RANSAC Primitive Extraction** | Robustly extracts geometric primitives including planes, spheres, cylinders, cones, and tori from point clouds. | `PrimitivesRansac` |
| **Point Cloud Simplification** | Reduces point cloud density while preserving important geometric features using methods such as voxel grid downsampling. | `PointCloudSimplification` |

## Surface Mesh Algorithms

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Mesh Simplification** | Reduces the number of triangles in a mesh while preserving the overall geometric shape using Quadric Error Metrics. | `SurfaceMeshSimplification` |
| **Mesh Smoothing** | Reduces noise in mesh geometry through various filtering techniques including Laplacian, Taubin, and bilateral filters. | `SurfaceMeshSmoothing` |
| **Incremental Remeshing** | Improves mesh quality by iteratively applying edge splits, collapses, swaps, and tangential smoothing operations. | `SurfaceMeshRemeshing` |
| **Hole Filling** | Automatically detects and fills holes in surface meshes with locally optimal triangulations to restore manifold connectivity. | `SurfaceMeshHoleFilling` |
| **Mesh Fairing** | Smooths specific mesh regions while satisfying boundary constraints to produce surfaces with minimal curvature or area. | `SurfaceMeshFairing` |
| **Mesh Triangulation** | Converts general polygonal faces into a set of triangles for rendering and processing compatibility. | `SurfaceMeshTriangulation` |
| **Mesh Subdivision** | Increases mesh resolution by applying refinement schemes such as Catmull-Clark, Loop, or Sqrt(3) subdivision. | `SurfaceMeshSubdivision` |
| **Mesh Parameterization** | Maps 3D mesh surfaces to a 2D domain using harmonic, conformal, or least-squares conformal mapping techniques. | `SurfaceMeshParameterization` |
| **Mesh Sampling** | Generates a set of points on a mesh surface using uniform, random, or importance-based sampling strategies. | `SurfaceMeshSampler` |
| **Curvature Estimation** | Computes Gaussian, mean, and principal curvatures at each vertex of the surface mesh. | `SurfaceMeshCurvature` |
| **Mesh Stitching** | Combines multiple disjoint mesh components into a single manifold surface by stitching their shared boundaries. | `SurfaceMeshStitching` |
| **Geodesic Distance** | Computes shortest path distances between vertices on a mesh surface using algorithms such as Dijkstra or the Fast Marching Method. | `SurfaceMeshGeodesic` |

## Polyhedral Mesh Algorithms

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Tetrahedralization** | Generates a 3D tetrahedral mesh from a closed surface boundary, typically utilizing the TetGen library for robust volume triangulation. | `SurfaceMeshTetrahedralization` |

## General Geometry & Utilities

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Delaunay Triangulation** | Computes the Delaunay triangulation for 2D or 3D point sets to create optimal triangular or tetrahedral meshes. | `Delaunay2D`, `Delaunay3D` |
| **Polygon Partition** | Decomposes complex polygons into simpler convex pieces to facilitate efficient geometric computations. | `PolygonPartition` |
| **Tessellation** | Robustly triangulates complex polygons including those with holes or self-intersections. | `Tessellator` |
| **Collision Detection** | Detects geometric intersections and computes distances between different 3D models or primitives. | `Collider` |
| **Spline Interpolation** | Performs cubic spline interpolation for smooth curve fitting and data interpolation across multiple dimensions. | `SplineInterpolation` |
| **KD-Tree** | Implements efficient spatial indexing for fast nearest neighbor searches and range queries in 3D space. | `KdTree`, `TriangleMeshKdTree` |

## CGAL-based Extensions (Optional)

| Algorithm | Description | Key Class/Function |
|-----------|-------------|--------------------|
| **Self-Intersection Detection**| Identifies all faces that intersect with other faces within the same mesh structure. | `SelfIntersection` |
| **Overlapping Face Detection**| Identifies pairs of faces that are nearly coplanar and overlap within a specified tolerance. | `OverlappingFaces` |
