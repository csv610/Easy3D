#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/surface_mesh_curvature.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Mesh Curvature Analysis");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file with curvature properties");
    args::ValueFlag<bool> compute_principal(parser, "principal", "Compute principal curvatures (default: true)", {'p', "principal"}, true);
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
        file_path = resource::directory() + "/data/mannequin.ply";
    }

    std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
    if (!mesh) {
        LOG(ERROR) << "failed to load mesh from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "mesh loaded: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    std::cout << "analyzing curvature..." << std::endl;

    SurfaceMeshCurvature analyzer(mesh.get());

    if (args::get(compute_principal)) {
        std::cout << "computing principal curvatures..." << std::endl;
        analyzer.analyze_tensor(2, true);

        auto k1 = mesh->get_vertex_property<float>("v:curvature:k1");
        auto k2 = mesh->get_vertex_property<float>("v:curvature:k2");
        if (k1 && k2) {
            float min_k1 = std::numeric_limits<float>::max(), max_k1 = -std::numeric_limits<float>::max();
            float min_k2 = std::numeric_limits<float>::max(), max_k2 = -std::numeric_limits<float>::max();
            for (auto v : mesh->vertices()) {
                min_k1 = std::min(min_k1, k1[v]);
                max_k1 = std::max(max_k1, k1[v]);
                min_k2 = std::min(min_k2, k2[v]);
                max_k2 = std::max(max_k2, k2[v]);
            }
            std::cout << "  k1 (min principal): [" << min_k1 << ", " << max_k1 << "]" << std::endl;
            std::cout << "  k2 (max principal): [" << min_k2 << ", " << max_k2 << "]" << std::endl;
        }
    }

    std::cout << "computing mean curvature..." << std::endl;
    analyzer.compute_mean_curvature();
    auto mean = mesh->get_vertex_property<float>("v:curvature:mean");
    if (mean) {
        float min_h = std::numeric_limits<float>::max(), max_h = -std::numeric_limits<float>::max();
        for (auto v : mesh->vertices()) {
            min_h = std::min(min_h, mean[v]);
            max_h = std::max(max_h, mean[v]);
        }
        std::cout << "  mean curvature: [" << min_h << ", " << max_h << "]" << std::endl;
    }

    std::cout << "computing Gauss curvature..." << std::endl;
    analyzer.compute_gauss_curvature();
    auto gauss = mesh->get_vertex_property<float>("v:curvature:gauss");
    if (gauss) {
        float min_g = std::numeric_limits<float>::max(), max_g = -std::numeric_limits<float>::max();
        for (auto v : mesh->vertices()) {
            min_g = std::min(min_g, gauss[v]);
            max_g = std::max(max_g, gauss[v]);
        }
        std::cout << "  Gauss curvature: [" << min_g << ", " << max_g << "]" << std::endl;
    }

    std::cout << "computing max absolute curvature..." << std::endl;
    analyzer.compute_max_abs_curvature();
    auto max_abs = mesh->get_vertex_property<float>("v:curv-max");
    if (max_abs) {
        float min_m = std::numeric_limits<float>::max(), max_m = -std::numeric_limits<float>::max();
        for (auto v : mesh->vertices()) {
            min_m = std::min(min_m, max_abs[v]);
            max_m = std::max(max_m, max_abs[v]);
        }
        std::cout << "  max absolute curvature: [" << min_m << ", " << max_m << "]" << std::endl;
    }

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_curvature.ply";
    }

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
        std::cout << "curvature properties stored in vertex properties:" << std::endl;
        std::cout << "  v:curvature:k1, v:curvature:k2 (principal)" << std::endl;
        std::cout << "  v:curvature:mean (mean)" << std::endl;
        std::cout << "  v:curvature:gauss (Gauss)" << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}
