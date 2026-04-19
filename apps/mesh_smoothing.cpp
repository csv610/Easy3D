#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/surface_mesh_smoothing.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Mesh Smoothing");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file");
    args::ValueFlag<int> iterations(parser, "iterations", "Number of iterations (default: 50)", {'i', "iterations"}, 50);
    args::ValueFlag<float> lambda(parser, "lambda", "Smoothing step size lambda (default: 0.5)", {'l', "lambda"}, 0.5f);
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

    int iter = args::get(iterations);
    float lambda_val = args::get(lambda);

    std::cout << "smoothing (iterations=" << iter << ", lambda=" << lambda_val << ")..." << std::endl;

    SurfaceMeshSmoothing smoother(mesh.get());
    smoother.explicit_smoothing(iter, false);

    std::cout << "smoothing complete: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_smooth.ply";
    }

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}