# CLI applications

# IO
add_executable(point_cloud_io ${Easy3D_ROOT}/applications/point_cloud_io.cpp)
target_include_directories(point_cloud_io PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(point_cloud_io PRIVATE easy3d::core easy3d::fileio easy3d::util)
set_target_properties(point_cloud_io PROPERTIES FOLDER "applications")

add_executable(surface_mesh_io ${Easy3D_ROOT}/applications/surface_mesh_io.cpp)
target_include_directories(surface_mesh_io PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(surface_mesh_io PRIVATE easy3d::core easy3d::fileio easy3d::util)
set_target_properties(surface_mesh_io PROPERTIES FOLDER "applications")

# Point Cloud Processing
add_executable(normal_estimation ${Easy3D_ROOT}/applications/normal_estimation.cpp)
target_include_directories(normal_estimation PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(normal_estimation PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(normal_estimation PROPERTIES FOLDER "applications")

add_executable(point_cloud_downsampling ${Easy3D_ROOT}/applications/point_cloud_downsampling.cpp)
target_include_directories(point_cloud_downsampling PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(point_cloud_downsampling PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(point_cloud_downsampling PROPERTIES FOLDER "applications")

# Surface Reconstruction
add_executable(surface_reconstruction ${Easy3D_ROOT}/applications/surface_reconstruction.cpp)
target_include_directories(surface_reconstruction PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(surface_reconstruction PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(surface_reconstruction PROPERTIES FOLDER "applications")

add_executable(plane_extraction ${Easy3D_ROOT}/applications/plane_extraction.cpp)
target_include_directories(plane_extraction PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(plane_extraction PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::renderer easy3d::util)
set_target_properties(plane_extraction PROPERTIES FOLDER "applications")

# Delaunay
add_executable(delaunay_2d ${Easy3D_ROOT}/applications/delaunay_2d.cpp)
target_include_directories(delaunay_2d PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(delaunay_2d PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(delaunay_2d PROPERTIES FOLDER "applications")

add_executable(delaunay_3d ${Easy3D_ROOT}/applications/delaunay_3d.cpp)
target_include_directories(delaunay_3d PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(delaunay_3d PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(delaunay_3d PROPERTIES FOLDER "applications")

# Mesh Processing
add_executable(mesh_smoothing ${Easy3D_ROOT}/applications/mesh_smoothing.cpp)
target_include_directories(mesh_smoothing PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_smoothing PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_smoothing PROPERTIES FOLDER "applications")

add_executable(mesh_simplification ${Easy3D_ROOT}/applications/mesh_simplification.cpp)
target_include_directories(mesh_simplification PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_simplification PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_simplification PROPERTIES FOLDER "applications")

add_executable(mesh_remeshing ${Easy3D_ROOT}/applications/mesh_remeshing.cpp)
target_include_directories(mesh_remeshing PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_remeshing PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_remeshing PROPERTIES FOLDER "applications")

add_executable(mesh_hole_filling ${Easy3D_ROOT}/applications/mesh_hole_filling.cpp)
target_include_directories(mesh_hole_filling PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_hole_filling PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_hole_filling PROPERTIES FOLDER "applications")

add_executable(mesh_fairing ${Easy3D_ROOT}/applications/mesh_fairing.cpp)
target_include_directories(mesh_fairing PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_fairing PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_fairing PROPERTIES FOLDER "applications")

add_executable(mesh_triangulation ${Easy3D_ROOT}/applications/mesh_triangulation.cpp)
target_include_directories(mesh_triangulation PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_triangulation PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_triangulation PROPERTIES FOLDER "applications")

add_executable(mesh_subdivision ${Easy3D_ROOT}/applications/mesh_subdivision.cpp)
target_include_directories(mesh_subdivision PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_subdivision PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_subdivision PROPERTIES FOLDER "applications")

add_executable(mesh_parameterization ${Easy3D_ROOT}/applications/mesh_parameterization.cpp)
target_include_directories(mesh_parameterization PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_parameterization PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_parameterization PROPERTIES FOLDER "applications")

add_executable(mesh_sampling ${Easy3D_ROOT}/applications/mesh_sampling.cpp)
target_include_directories(mesh_sampling PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_sampling PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_sampling PROPERTIES FOLDER "applications")

add_executable(mesh_curvature ${Easy3D_ROOT}/applications/mesh_curvature.cpp)
target_include_directories(mesh_curvature PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_curvature PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_curvature PROPERTIES FOLDER "applications")

add_executable(mesh_stitching ${Easy3D_ROOT}/applications/mesh_stitching.cpp)
target_include_directories(mesh_stitching PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(mesh_stitching PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(mesh_stitching PROPERTIES FOLDER "applications")

add_executable(tetrahedralization ${Easy3D_ROOT}/applications/tetrahedralization.cpp)
target_include_directories(tetrahedralization PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(tetrahedralization PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(tetrahedralization PROPERTIES FOLDER "applications")

# Geometry
add_executable(polygon_partition ${Easy3D_ROOT}/applications/polygon_partition.cpp)
target_include_directories(polygon_partition PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
target_link_libraries(polygon_partition PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::util)
set_target_properties(polygon_partition PROPERTIES FOLDER "applications")

# CGAL Extensions (disabled - requires proper CGAL include paths)
# if (Easy3D_ENABLE_CGAL)
#     add_executable(self_intersection ${Easy3D_ROOT}/applications/self_intersection.cpp)
#     target_include_directories(self_intersection PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
#     target_compile_definitions(self_intersection PRIVATE HAS_CGAL)
#     target_link_libraries(self_intersection PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::algo_ext easy3d::util)
#     set_target_properties(self_intersection PROPERTIES FOLDER "applications")
#
#     add_executable(overlapping_faces ${Easy3D_ROOT}/applications/overlapping_faces.cpp)
#     target_include_directories(overlapping_faces PRIVATE ${Easy3D_INCLUDE_DIR} ${Easy3D_THIRD_PARTY}/args)
#     target_compile_definitions(overlapping_faces PRIVATE HAS_CGAL)
#     target_link_libraries(overlapping_faces PRIVATE easy3d::core easy3d::fileio easy3d::algo easy3d::algo_ext easy3d::util)
#     set_target_properties(overlapping_faces PROPERTIES FOLDER "applications")
# endif ()

if (Easy3D_ENABLE_QT)
    add_subdirectory(Mapple)
endif ()