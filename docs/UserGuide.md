# Easy3D-M1 User Guide: A Journey into 3D Geometry Processing

Welcome to the world of 3D geometry processing! This guide is designed for undergraduate students and developers who want to learn how to represent, process, and visualize 3D data using the **Easy3D-M1** library.

---

## Table of Contents
1. [Introduction to Easy3D-M1](#1-introduction-to-easy3d-m1)
2. [Fundamental Data Structures](#2-fundamental-data-structures)
3. [Reading and Writing 3D Data](#3-reading-and-writing-3d-data)
4. [Mastering Point Clouds](#4-mastering-point-clouds)
5. [The Art of Surface Meshes](#5-the-art-of-surface-meshes)
6. [Visualizing Your World](#6-visualizing-your-world)
7. [Advanced Geometry Processing](#7-advanced-geometry-processing)
8. [Building Your Own Applications](#8-building-your-own-applications)

---

## 1. Introduction to Easy3D-M1

### What is Easy3D-M1?
Easy3D-M1 is a lightweight, efficient, and easy-to-use C++ library for 3D modeling, geometry processing, and rendering. Originally developed by Liangliang Nan, this version is specifically optimized for **macOS M1 (Apple Silicon)** and focuses on modern C++ practices using smart pointers.

### Why Study Geometry Processing?
From video games and animated movies to architectural design and autonomous driving, 3D geometry is everywhere. Geometry processing is the field that studies how to efficiently represent and manipulate these 3D shapes.

---

## 2. Fundamental Data Structures

Easy3D-M1 provides four primary data structures to represent different types of 3D geometry:

### 2.1 Point Cloud (`easy3d::PointCloud`)
A collection of points in 3D space. Each point has coordinates $(x, y, z)$.
*   **Use case:** Raw data from LiDAR scanners or depth cameras.
*   **Key tutorial:** `Tutorial_101_PointCloud`

### 2.2 Surface Mesh (`easy3d::SurfaceMesh`)
A collection of vertices, edges, and faces (usually triangles or quads) that define the "skin" of an object.
*   **Use case:** Computer graphics, CAD, and 3D printing.
*   **Key tutorial:** `Tutorial_104_SurfaceMesh`

### 2.3 Graph (`easy3d::Graph`)
A set of nodes connected by edges.
*   **Use case:** Network analysis, skeleton extraction.
*   **Key tutorial:** `Tutorial_108_Graph`

### 2.4 Polyhedral Mesh (`easy3d::PolyMesh`)
A volumetric representation consisting of cells (e.g., tetrahedra).
*   **Use case:** Finite Element Method (FEM) simulations, physics engines.
*   **Key tutorial:** `Tutorial_112_PolyMesh`

---

## 3. Reading and Writing 3D Data

Before you can process geometry, you need to get it into your program. Easy3D-M1 supports many formats:

### Common File Formats
*   **PLY:** Standard format for point clouds and meshes.
*   **OBJ:** Widely used in 3D modeling software.
*   **OFF:** Simple format often used in academia.
*   **LAS/LAZ:** Standard for aerial LiDAR data.

### Loading a Point Cloud
```cpp
#include <easy3d/fileio/point_cloud_io.h>

// Use unique_ptr for automatic memory management
auto cloud = std::unique_ptr<easy3d::PointCloud>(
    easy3d::PointCloudIO::load("my_data.ply")
);

if (cloud) {
    std::cout << "Loaded " << cloud->n_vertices() << " points." << std::endl;
}
```

---

## 4. Mastering Point Clouds

Point clouds are often the starting point of any 3D pipeline.

### 4.1 Normal Estimation
Normals tell us which way a surface is facing at a specific point. They are essential for lighting and surface reconstruction.
*   **Command:** `normal_estimation input.ply output.ply`
*   **Algorithm:** Principal Component Analysis (PCA) on local neighborhoods.

### 4.2 Downsampling
Scanning often produces millions of points. Downsampling reduces this number while preserving the shape.
*   **Method:** Voxel Grid filtering.
*   **Command:** `point_cloud_downsampling input.ply output.ply --size 0.05`

### 4.3 Surface Reconstruction
Turning points into a solid surface.
*   **Method:** Poisson Surface Reconstruction.
*   **Command:** `surface_reconstruction input.ply output.obj`

---

## 5. The Art of Surface Meshes

Once you have a mesh, you can perform more complex operations.

### 5.1 Simplification (Decimation)
Reducing the triangle count while keeping the visual quality.
*   **Algorithm:** Quadric Error Metrics (QEM).
*   **Command:** `mesh_simplification input.obj output.obj --percent 0.5`

### 5.2 Smoothing
Removing noise from the surface.
*   **Methods:** Laplacian (classic), Taubin (preserves size).
*   **Command:** `mesh_smoothing input.obj output.obj --iter 10`

### 5.3 Remeshing
Improving the quality of individual triangles (making them more equilateral).
*   **Command:** `mesh_remeshing input.obj output.obj`

---

## 6. Visualizing Your World

Easy3D-M1 includes a powerful OpenGL-based viewer.

### Basic Usage
```cpp
#include <easy3d/viewer/viewer.h>

int main() {
    easy3d::initialize();
    easy3d::Viewer viewer("My First 3D App");
    viewer.add_model("bunny.obj");
    return viewer.run();
}
```

### Advanced Rendering Features
*   **Ambient Occlusion:** Adds realistic shadows in crevices.
*   **Eye-Dome Lighting:** Makes point clouds look like solid surfaces.
*   **Transparency:** Supports complex overlapping transparent objects.

---

## 7. Advanced Geometry Processing

### 7.1 Delaunay Triangulation
The "perfect" triangulation where no point is inside the circumcircle of any triangle.
*   **Applications:** Terrain modeling, mesh generation.

### 7.2 RANSAC Primitive Extraction
Automatically finding planes, cylinders, and spheres in noisy point clouds.
*   **Command:** `plane_extraction input.ply`

---

## 8. Building Your Own Applications

When writing new tools with Easy3D-M1, follow these "Pro Tips":

1.  **Always use `std::unique_ptr`**: Never use `new` or `delete` manually.
2.  **Initialize Easy3D**: Call `easy3d::initialize()` at the start of `main()`.
3.  **Check the Tutorials**: The `tutorials/` folder is your best friend. Every major feature has a corresponding tutorial.
4.  **Use CLI Tools**: For batch processing, use the pre-built tools in the `bin/` directory.

### Example Template
```cpp
#include <easy3d/core/point_cloud.h>
#include <easy3d/util/initializer.h>
#include <memory>

int main(int argc, char** argv) {
    easy3d::initialize();
    
    // Your logic here...
    
    return 0;
}
```

---

*Happy Coding! The world is your 3D oyster.*
