#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/surface_mesh_simplification.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Mesh Simplification");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file");
    args::ValueFlag<float> target_ratio(parser, "ratio", "Target face ratio (0.1 = 10%% of original) (default: 0.1)", {'r', "ratio"}, 0.1f);
    args::ValueFlag<float> agg_factors(parser, "aggression", "Aggression factor (default: 5.0)", {'a', "aggression"}, 5.0f);
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

    int original_faces = mesh->n_faces();
    std::cout << "mesh loaded: " << mesh->n_vertices() << " vertices, " << original_faces << " faces" << std::endl;

    float ratio = args::get(target_ratio);
    float agg = args::get(agg_factors);

    int target_vertices = static_cast<int>(mesh->n_vertices() * ratio);
    std::cout << "simplifying to " << target_vertices << " vertices (ratio=" << ratio << ")..." << std::endl;

    const int normal_deviation = 180;
    const int aspect_ratio = 10;

    SurfaceMeshSimplification simplifier(mesh.get());
    simplifier.initialize(aspect_ratio, 0.0f, 0, normal_deviation, 0.0f);
    simplifier.simplify(target_vertices);

    std::cout << "simplified to: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;
    std::cout << "reduction: " << (100.0f * (1.0f - (float)mesh->n_faces() / original_faces)) << "%%" << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_simplified.ply";
    }

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}