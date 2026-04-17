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

The `applications/` directory contains command-line tools:

| Tool | Description |
|------|-------------|
| `point_cloud_io` | Read/write point cloud files |
| `surface_mesh_io` | Read/write surface mesh files |
| `normal_estimation` | Estimate point cloud normals |
| `surface_reconstruction` | Poisson surface reconstruction |
| `plane_extraction` | RANSAC-based plane extraction |
| `point_cloud_downsampling` | Voxel grid downsampling |
| `mesh_simplification` | Simplify surface meshes |
| `mesh_smoothing` | Smooth surface meshes |
| `mesh_remeshing` | Remesh surface meshes |
| And more... |

## Building

```bash
cd Easy3D
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(sysctl -n hw.ncpu)
```

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

## Comparison with Other Libraries

| Feature | Easy3D-M1 | CGAL | libIGL | PCL |
|---------|-----------|------|--------|-----|
| **Rendering** | Built-in OpenGL | None | None | Basic |
| **Ease of Use** | High | Complex | Medium | Steep |
| **CLI Focus** | Yes | No | No | Partial |
| **Dependencies** | Minimal | Heavy | Eigen | VTK/Boost |

## License

GNU General Public License v3. See [LICENSE](LICENSE).

## Acknowledgments

Based on the original [Easy3D](https://github.com/LiangliangNan/Easy3D) by Liangliang Nan.