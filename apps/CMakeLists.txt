# CLI apps

# IO
add_executable(point_cloud_io ${Easy3D_ROOT}/apps/point_cloud_io.cpp)
target_include_directories(point_cloud_io PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(point_cloud_io PRIVATE easy3d::core easy3d::fileio easy3d::util)
set_target_properties(point_cloud_io PROPERTIES FOLDER "apps")

add_executable(surface_mesh_io ${Easy3D_ROOT}/apps/surface_mesh_io.cpp)
target_include_directories(surface_mesh_io PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(surface_mesh_io PRIVATE easy3d::core easy3d::fileio easy3d::util)
set_target_properties(surface_mesh_io PROPERTIES FOLDER "apps")

# Point Cloud Processing
add_executable(normal_estimation ${Easy3D_ROOT}/apps/normal_estimation.cpp)
target_include_directories(normal_estimation PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(normal_estimation PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(normal_estimation PROPERTIES FOLDER "apps")

add_executable(point_cloud_downsampling ${Easy3D_ROOT}/apps/point_cloud_downsampling.cpp)
target_include_directories(point_cloud_downsampling PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(point_cloud_downsampling PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(point_cloud_downsampling PROPERTIES FOLDER "apps")

# Surface Reconstruction
add_executable(surface_reconstruction ${Easy3D_ROOT}/apps/surface_reconstruction.cpp)
target_include_directories(surface_reconstruction PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(surface_reconstruction PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(surface_reconstruction PROPERTIES FOLDER "apps")

add_executable(plane_extraction ${Easy3D_ROOT}/apps/plane_extraction.cpp)
target_include_directories(plane_extraction PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(plane_extraction PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::renderer easy3d::util)
set_target_properties(plane_extraction PROPERTIES FOLDER "apps")

# Delaunay
add_executable(delaunay_2d ${Easy3D_ROOT}/apps/delaunay_2d.cpp)
target_include_directories(delaunay_2d PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(delaunay_2d PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(delaunay_2d PROPERTIES FOLDER "apps")

add_executable(delaunay_3d ${Easy3D_ROOT}/apps/delaunay_3d.cpp)
target_include_directories(delaunay_3d PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(delaunay_3d PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(delaunay_3d PROPERTIES FOLDER "apps")

# Mesh Processing
add_executable(mesh_smoothing ${Easy3D_ROOT}/apps/mesh_smoothing.cpp)
target_include_directories(mesh_smoothing PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_smoothing PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_smoothing PROPERTIES FOLDER "apps")

add_executable(mesh_simplification ${Easy3D_ROOT}/apps/mesh_simplification.cpp)
target_include_directories(mesh_simplification PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_simplification PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_simplification PROPERTIES FOLDER "apps")

add_executable(mesh_remeshing ${Easy3D_ROOT}/apps/mesh_remeshing.cpp)
target_include_directories(mesh_remeshing PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_remeshing PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_remeshing PROPERTIES FOLDER "apps")

add_executable(mesh_hole_filling ${Easy3D_ROOT}/apps/mesh_hole_filling.cpp)
target_include_directories(mesh_hole_filling PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_hole_filling PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_hole_filling PROPERTIES FOLDER "apps")

add_executable(mesh_fairing ${Easy3D_ROOT}/apps/mesh_fairing.cpp)
target_include_directories(mesh_fairing PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_fairing PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_fairing PROPERTIES FOLDER "apps")

add_executable(mesh_triangulation ${Easy3D_ROOT}/apps/mesh_triangulation.cpp)
target_include_directories(mesh_triangulation PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_triangulation PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_triangulation PROPERTIES FOLDER "apps")

add_executable(mesh_subdivision ${Easy3D_ROOT}/apps/mesh_subdivision.cpp)
target_include_directories(mesh_subdivision PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_subdivision PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_subdivision PROPERTIES FOLDER "apps")

add_executable(mesh_parameterization ${Easy3D_ROOT}/apps/mesh_parameterization.cpp)
target_include_directories(mesh_parameterization PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_parameterization PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_parameterization PROPERTIES FOLDER "apps")

add_executable(mesh_sampling ${Easy3D_ROOT}/apps/mesh_sampling.cpp)
target_include_directories(mesh_sampling PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_sampling PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_sampling PROPERTIES FOLDER "apps")

add_executable(mesh_curvature ${Easy3D_ROOT}/apps/mesh_curvature.cpp)
target_include_directories(mesh_curvature PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_curvature PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_curvature PROPERTIES FOLDER "apps")

add_executable(mesh_stitching ${Easy3D_ROOT}/apps/mesh_stitching.cpp)
target_include_directories(mesh_stitching PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_stitching PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_stitching PROPERTIES FOLDER "apps")

add_executable(tetrahedralization ${Easy3D_ROOT}/apps/tetrahedralization.cpp)
target_include_directories(tetrahedralization PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(tetrahedralization PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(tetrahedralization PROPERTIES FOLDER "apps")

# Geometry
add_executable(polygon_partition ${Easy3D_ROOT}/apps/polygon_partition.cpp)
target_include_directories(polygon_partition PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(polygon_partition PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(polygon_partition PROPERTIES FOLDER "apps")

# CGAL Extensions (disabled - requires proper CGAL include paths)
# if (Easy3D_ENABLE_CGAL)
#     add_executable(self_intersection ${Easy3D_ROOT}/apps/self_intersection.cpp)
#     target_include_directories(self_intersection PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
#     target_compile_definitions(self_intersection PRIVATE HAS_CGAL)
#     target_link_libraries(self_intersection PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::algo_ext easy3d::util)
#     set_target_properties(self_intersection PROPERTIES FOLDER "apps")
#
#     add_executable(overlapping_faces ${Easy3D_ROOT}/apps/overlapping_faces.cpp)
#     target_include_directories(overlapping_faces PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
#     target_compile_definitions(overlapping_faces PRIVATE HAS_CGAL)
#     target_link_libraries(overlapping_faces PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::algo_ext easy3d::util)
#     set_target_properties(overlapping_faces PROPERTIES FOLDER "apps")
# endif ()

if (Easy3D_ENABLE_QT)
    add_subdirectory(Mapple)
endif ()
