#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/surface_mesh_subdivision.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Mesh Subdivision");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output subdivided mesh file");
    args::ValueFlag<std::string> method(parser, "method", "Subdivision method: loop, sqrt3 (default: loop)", {'m', "method"}, "loop");
    args::ValueFlag<int> levels(parser, "levels", "Number of subdivision levels (default: 1)", {'l', "levels"}, 1);
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
        file_path = resource::directory() + "/data/sphere.obj";
    }

    std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
    if (!mesh) {
        LOG(ERROR) << "failed to load mesh from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "mesh loaded: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    std::string method_name = args::get(method);
    int levels_val = args::get(levels);

    std::cout << "subdividing (method=" << method_name << ", levels=" << levels_val << ")..." << std::endl;

    for (int i = 0; i < levels_val; ++i) {
        if (method_name == "sqrt3") {
            SurfaceMeshSubdivision::sqrt3(mesh.get());
        } else if (method_name == "loop") {
            SurfaceMeshSubdivision::loop(mesh.get());
        } else {
            SurfaceMeshSubdivision::catmull_clark(mesh.get());
        }
    }

    std::cout << "subdivided to: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_subdivided.obj";
    }

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}