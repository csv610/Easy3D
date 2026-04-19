#include <easy3d/core/point_cloud.h>
#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/delaunay_2d.h>
#include <easy3d/algo/point_cloud_simplification.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <easy3d/util/file_system.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Delaunay Triangulation 2D");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud or mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file");
    args::HelpFlag help(parser, "help", "Display this help message", {'h', "help"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    initialize();

    std::string file_path;
    if (input_file) {
        file_path = args::get(input_file);
    } else {
        file_path = resource::directory() + "/data/bunny.bin";
    }

    std::unique_ptr<PointCloud> cloud;
    std::string input_ext = file_system::extension(file_path, true);

    // Try loading as point cloud first for typical PC formats
    if (input_ext == "ply" || input_ext == "bin" || input_ext == "xyz" || input_ext == "las" || input_ext == "laz") {
        cloud.reset(PointCloudIO::load(file_path));
        if (cloud) {
            std::cout << "point cloud loaded: " << cloud->n_vertices() << " points" << std::endl;
        }
    } 

    // If not loaded yet, try as surface mesh and convert to point cloud
    if (!cloud) {
        std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
        if (mesh) {
            std::cout << "surface mesh loaded: " << mesh->n_vertices() << " vertices" << std::endl;
            cloud = std::make_unique<PointCloud>();
            for (auto v : mesh->vertices()) {
                cloud->add_vertex(mesh->position(v));
            }
        }
    }

    if (!cloud || cloud->n_vertices() == 0) {
        LOG(ERROR) << "failed to load points from: " << file_path;
        return EXIT_FAILURE;
    }

    // Remove duplicates
    std::cout << "removing duplicate points..." << std::endl;
    auto to_delete = PointCloudSimplification::uniform_simplification(cloud.get(), 1e-6f);
    if (!to_delete.empty()) {
        cloud->delete_vertices(to_delete);
        cloud->collect_garbage();
        std::cout << "  " << to_delete.size() << " duplicate points removed. " << cloud->n_vertices() << " unique points remain." << std::endl;
    } else {
        std::cout << "  no duplicate points found." << std::endl;
    }

    std::vector<vec2> pts;
    auto points_prop = cloud->get_vertex_property<vec3>("v:point");
    for (auto v : cloud->vertices()) {
        pts.push_back(vec2(points_prop[v].x, points_prop[v].y));
    }

    std::cout << "computing Delaunay triangulation 2D..." << std::endl;
    Delaunay2 delaunay;
    delaunay.set_vertices(pts);

    std::cout << "triangulation result:" << std::endl;
    std::cout << "  vertices: " << delaunay.nb_vertices() << std::endl;
    std::cout << "  triangles: " << delaunay.nb_triangles() << std::endl;

    std::unique_ptr<SurfaceMesh> out_mesh(new SurfaceMesh);
    for (unsigned int i = 0; i < delaunay.nb_vertices(); ++i) {
        out_mesh->add_vertex(vec3(delaunay.vertex(i).x, delaunay.vertex(i).y, 0.0f));
    }

    const int* tri_to_v = delaunay.tri_to_v();
    for (unsigned int i = 0; i < delaunay.nb_triangles(); ++i) {
        out_mesh->add_triangle(
            SurfaceMesh::Vertex(tri_to_v[3 * i]),
            SurfaceMesh::Vertex(tri_to_v[3 * i + 1]),
            SurfaceMesh::Vertex(tri_to_v[3 * i + 2])
        );
    }

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_delaunay_2d.obj";
    }

    if (SurfaceMeshIO::save(out_path, out_mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}