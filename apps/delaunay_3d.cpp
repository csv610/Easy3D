#include <easy3d/core/point_cloud.h>
#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/delaunay_3d.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <easy3d/util/file_system.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Delaunay Triangulation 3D");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud or mesh file");
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

    std::vector<vec3> pts;
    std::string input_ext = file_system::extension(file_path, true);

    // Try loading as point cloud first for typical PC formats
    if (input_ext == "ply" || input_ext == "bin" || input_ext == "xyz" || input_ext == "las" || input_ext == "laz") {
        std::unique_ptr<PointCloud> cloud(PointCloudIO::load(file_path));
        if (cloud) {
            std::cout << "point cloud loaded: " << cloud->n_vertices() << " points" << std::endl;
            pts = cloud->points();
        }
    }

    // If not loaded yet, try as surface mesh
    if (pts.empty()) {
        std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
        if (mesh) {
            std::cout << "surface mesh loaded: " << mesh->n_vertices() << " vertices" << std::endl;
            for (auto v : mesh->vertices()) {
                pts.push_back(mesh->position(v));
            }
        }
    }

    if (pts.empty()) {
        LOG(ERROR) << "failed to load points from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "computing Delaunay triangulation 3D..." << std::endl;
    Delaunay3 delaunay;
    delaunay.set_vertices(pts);

    std::cout << "triangulation result:" << std::endl;
    std::cout << "  vertices: " << delaunay.nb_vertices() << std::endl;
    std::cout << "  tetrahedra: " << delaunay.nb_tets() << std::endl;

    std::cout << "Note: 3D Delaunay mesh is computed in memory. Use other tools to convert to file." << std::endl;
    std::cout << "To save as surface mesh, convert the boundary faces to a SurfaceMesh." << std::endl;

    return EXIT_SUCCESS;
}