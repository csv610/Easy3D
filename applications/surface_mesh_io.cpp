#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>
#include <memory>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Surface Mesh IO - Load and save surface mesh files");

    args::Positional<std::string> input_file(parser, "input", "Input mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output file (optional)");
    args::HelpFlag help(parser, "help", "Display this help message", {'h', 'help'});

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
        file_path = resource::directory() + "/data/sphere.obj";
    }

    std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
    if (!mesh) {
        LOG(ERROR) << "failed to load mesh from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "mesh loaded:" << std::endl;
    std::cout << "  vertices: " << mesh->n_vertices() << std::endl;
    std::cout << "  edges: " << mesh->n_edges() << std::endl;
    std::cout << "  faces: " << mesh->n_faces() << std::endl;

    if (output_file) {
        std::string out_path = args::get(output_file);
        if (SurfaceMeshIO::save(out_path, mesh.get())) {
            std::cout << "saved to: " << out_path << std::endl;
        } else {
            std::cerr << "failed to save to: " << out_path << std::endl;
        }
    }

    return EXIT_SUCCESS;
}