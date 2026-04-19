#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/surface_mesh_remeshing.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Mesh Remeshing");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file");
    args::ValueFlag<float> edge_size(parser, "edge-size", "Target edge size (default: 0.0 = adaptive)", {'e', "edge-size"}, 0.0f);
    args::Flag adaptive(parser, "adaptive", "Use adaptive remeshing (default: false)", {'a', "adaptive"});
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
        file_path = resource::directory() + "/data/bunny.ply";
    }

    std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
    if (!mesh) {
        LOG(ERROR) << "failed to load mesh from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "mesh loaded: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    float target_edge = args::get(edge_size);
    bool is_adaptive = args::get(adaptive);

    std::cout << "remeshing (edge_size=" << target_edge << ", adaptive=" << (is_adaptive ? "true" : "false") << ")..." << std::endl;

    SurfaceMeshRemeshing remesher(mesh.get());
    if (is_adaptive) {
        if (target_edge > 0) {
            remesher.adaptive_remeshing(target_edge * 0.5f, target_edge * 2.0f, target_edge * 0.01f, 10);
        } else {
            remesher.adaptive_remeshing(0.001f, 0.1f, 0.001f, 10);
        }
    } else {
        float edge_length = target_edge > 0 ? target_edge : 0.0f;
        remesher.uniform_remeshing(edge_length, 10);
    }

    std::cout << "remeshed to: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_remeshed.ply";
    }

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}