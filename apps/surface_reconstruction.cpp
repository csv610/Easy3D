#include <easy3d/core/point_cloud.h>
#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <memory>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/point_cloud_poisson_reconstruction.h>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Surface Reconstruction - Poisson surface reconstruction from point cloud");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file");
    args::ValueFlag<int> depth(parser, "depth", "Reconstruction depth (default: 6)", {'d', "depth"}, 6);
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
        file_path = resource::directory() + "/data/polyhedron.bin";
    }

    std::unique_ptr<PointCloud> cloud(PointCloudIO::load(file_path));
    if (!cloud) {
        LOG(ERROR) << "failed to load point cloud from: " << file_path;
        return EXIT_FAILURE;
    }

    auto normals = cloud->get_vertex_property<vec3>("v:normal");
    if (!normals) {
        std::cerr << "Error: Point cloud has no normal information." << std::endl;
        std::cerr << "Please run normal_estimation first to compute normals." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "point cloud loaded: " << cloud->n_vertices() << " points" << std::endl;

    int d = args::get(depth);
    std::cout << "running Poisson surface reconstruction (depth = " << d << ")..." << std::endl;

    PoissonReconstruction algo;
    algo.set_depth(d);

    std::unique_ptr<Model> surface(algo.apply(cloud.get()));
    if (!surface) {
        std::cerr << "failed to reconstruct surface" << std::endl;
        return EXIT_FAILURE;
    }

    auto* mesh = dynamic_cast<SurfaceMesh*>(surface.get());
    if (!mesh) {
        std::cerr << "reconstruction did not return a surface mesh" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "reconstruction complete:" << std::endl;
    std::cout << "  vertices: " << mesh->n_vertices() << std::endl;
    std::cout << "  edges: " << mesh->n_edges() << std::endl;
    std::cout << "  faces: " << mesh->n_faces() << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_surface.obj";
    }

    if (SurfaceMeshIO::save(out_path, mesh)) {
        std::cout << "mesh saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save mesh to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}