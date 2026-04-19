<p align="right">
    <b> <img src="https://img.shields.io/badge/platforms-macOS%20M1-blue" title="Platform: macOS M1 (Apple Silicon)"/> </b> <br>
    <b> <img src="https://img.shields.io/badge/license-GPL-blue" title="license-GPL"/> </b> <br>
</p>

# Easy3D-M1

A port of [Easy3D](https://github.com/LiangliangNan/Easy3D) for macOS M1 (Apple Silicon). This version is optimized for ARM-based Macs and focuses on **command-line applications** using modern C++ with smart pointers.

## What is Easy3D?

Easy3D is an open-source C++ library for 3D modeling, geometry processing, and rendering. It provides:

- **Data structures**: Point clouds, surface meshes, polyhedral meshes, and graphs
- **Algorithms**: Normal estimation, Poisson reconstruction, RANSAC, mesh simplification/smoothing/remeshing, etc.
- **Rendering**: OpenGL-based rendering with shaders, shadows, ambient occlusion, transparency

## Why Easy3D-M1?

This port differs from the original:
- Optimized for macOS M1 (ARM64)
- Focus on CLI tools instead of GUI applications
- Modern C++ with smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Simplified build process

## CLI Applications

The `applications/` directory contains a comprehensive set of command-line tools for 3D data processing:

### Input / Output
| Tool | Description |
|------|-------------|
| `point_cloud_io` | Read/write point cloud files (PLY, OBJ, OFF, etc.) |
| `surface_mesh_io` | Read/write surface mesh files (PLY, OBJ, OFF, etc.) |

### Point Cloud Processing
| Tool | Description |
|------|-------------|
| `normal_estimation` | Estimate normals for point clouds |
| `point_cloud_downsampling` | Voxel-grid based downsampling |
| `plane_extraction` | RANSAC-based plane extraction from point clouds |
| `surface_reconstruction` | Poisson surface reconstruction from oriented points |

### Mesh Processing
| Tool | Description |
|------|-------------|
| `mesh_simplification` | Decimate surface meshes while preserving geometry |
| `mesh_smoothing` | Reduce noise in surface meshes (Laplacian, Taubin, etc.) |
| `mesh_remeshing` | Improve mesh quality through incremental remeshing |
| `mesh_hole_filling` | Automatically detect and fill holes in surface meshes |
| `mesh_fairing` | Smooth surface meshes while preserving boundary |
| `mesh_triangulation` | Convert general polygons to triangles |
| `mesh_subdivision` | Increase mesh resolution (Catmull-Clark, Loop, etc.) |
| `mesh_parameterization` | Map 3D meshes to 2D domains (UV mapping) |
| `mesh_sampling` | Generate points on mesh surfaces |
| `mesh_curvature` | Compute Gaussian, mean, and principal curvatures |
| `mesh_stitching` | Stitch multiple surface meshes together |
| `tetrahedralization` | Generate tetrahedral meshes from surface boundaries |

### Geometry & Triangulation
| Tool | Description |
|------|-------------|
| `delaunay_2d` | Compute 2D Delaunay triangulation |
| `delaunay_3d` | Compute 3D Delaunay triangulation |
| `polygon_partition` | Partition polygons into convex pieces |

## Building

Easy3D-M1 defaults to **Release** build with **Tests**, **Documentation**, **CGAL**, and **FFmpeg** support enabled.

```bash
cd Easy3D
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

## Testing

To run the unit tests:

1. Build the tests (enabled by default):
   ```bash
   make Tests -j$(sysctl -n hw.ncpu)
   ```
2. Run the tests:
   ```bash
   ./bin/Tests
   ```

Note: Some tests are automated, while others open a window for visual verification or require user interaction.

## Contributing

Contribute by writing CLI applications using smart pointers:

```cpp
#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>

int main(int argc, char** argv) {
    auto cloud = std::unique_ptr<easy3d::PointCloud>(
        easy3d::PointCloudIO::load("input.ply")
    );
    
    if (!cloud) {
        std::cerr << "Failed to load point cloud\n";
        return 1;
    }
    
    // Process...
    
    easy3d::PointCloudIO::save(cloud.get(), "output.ply");
    return 0;
}
```

Guidelines:
- Use `std::unique_ptr` for ownership
- Use raw pointers only for non-owning references
- Prefer `std::make_unique` and `std::make_shared`
- Follow C++14/17 best practices
- Ensure macOS M1 compatibility

## Distinctive Features

Easy3D stands out among geometry processing libraries for several reasons:

### All-in-One Solution
Unlike CGAL (algorithms only) or libIGL (algorithms only), Easy3D provides:
- **Data structures** for point clouds, surface meshes, poly meshes, and graphs
- **Algorithms** for geometry processing (reconstruction, simplification, smoothing, etc.)
- **Rendering** with modern OpenGL shaders (no fixed-function pipeline)

### Built-in Rendering
- Point/line imposters with hardware acceleration
- Ambient occlusion (SSAO), hard/soft shadows
- Eye-dome lighting for point clouds without normals
- Transparency (average color blending, dual depth peeling)
- Cross-platform viewer supporting GLFW, Qt, and wxWidgets

### Educational & Production-Ready
- Extensive tutorials with detailed code explanations
- Well-documented API
- Ready-to-use GUI application (Mapple)
- 20+ command-line tools for common tasks

### Minimal Dependencies
- Core requires only CMake and a C++11 compiler
- Optional: CGAL (advanced algorithms), Qt (GUI)
- Bundled 3rd-party sources for easy building

## Why Adopt Easy3D?

| Reason | Description |
|--------|-------------|
| **Quick Prototyping** | Few lines of code to load, process, and visualize 3D data |
| **Learning Curve** | Gentle - great for students learning 3D graphics |
| **Single Library** | No need to combine multiple libraries for geometry + rendering |
| **Modern C++** | Clean API with smart pointers, no raw memory management |
| **macOS M1 Ready** | Native ARM64 support, optimized for Apple Silicon |
| **CLI-First** | Perfect for batch processing and pipelines |

## Comparison with Other Libraries

| Feature | Easy3D-M1 | CGAL | libIGL | PCL |
|---------|-----------|------|--------|-----|
| **Primary Focus** | All-in-one | Computational Geometry | Geometry Algorithms | Point Cloud Processing |
| **Rendering** | Built-in OpenGL + shaders | None | None (matplotlib/MATLAB) | Basic PCL Viewer |
| **Data Structures** | Point cloud, mesh, graph, volume | Complex geometric kernels | Minimal mesh | Octrees, point clouds |
| **Ease of Use** | Very High | Complex | Medium | Steep |
| **Dependencies** | Minimal | Boost, GMP, MPFR | Eigen | VTK, Boost, FLANN |
| **Learning Curve** | Low | High | Medium | High |
| **GUI/Viewer** | Built-in (GLFW/Qt/wxWidgets) | None | None | PCL Viewer |
| **CLI Tools** | 20+ ready-to-use | None | None | Partial |
| **Target Platform** | macOS M1 (ARM64) | Cross-platform | Cross-platform | Cross-platform |

### When to Choose Easy3D-M1:
- You need **both** geometry processing **and** visualization
- You want **quick results** with minimal code
- You prefer **CLI tools** for batch processing
- You're on **macOS M1** and need native performance

### When to Choose Alternatives:
- **CGAL**: When you need exact arithmetic and advanced computational geometry
- **libIGL**: When you want header-only and prefer Python prototyping
- **PCL**: When your primary focus is point cloud robotics/vision

## License

GNU General Public License v3. See [LICENSE](LICENSE).

## Acknowledgments

Based on the original [Easy3D](https://github.com/LiangliangNan/Easy3D) by Liangliang Nan.