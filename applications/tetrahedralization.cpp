#include <easy3d/core/surface_mesh.h>
#include <easy3d/core/poly_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/fileio/poly_mesh_io.h>
#include <easy3d/algo/surface_mesh_tetrahedralization.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Mesh Tetrahedralization");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output tetrahedral mesh file");
    args::ValueFlag<float> radius_edge(parser, "radius-edge", "Maximum radius-edge ratio (default: 2.0)", {'r', "radius-edge"}, 2.0f);
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

    float radius_edge_ratio = args::get(radius_edge);

    std::cout << "tetrahedralizing (max radius-edge ratio = " << radius_edge_ratio << ")..." << std::endl;

    SurfaceMeshTetrehedralization tet;
    tet.set_allow_steiner_points_on_boundary(true);
    tet.set_max_tet_shape(radius_edge_ratio);

    std::unique_ptr<PolyMesh> poly(tet.apply(mesh.get()));

    if (!poly) {
        std::cerr << "failed to tetrahedralize" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "tetrahedralization result:" << std::endl;
    std::cout << "  vertices: " << poly->n_vertices() << std::endl;
    std::cout << "  cells: " << poly->n_cells() << std::endl;
    std::cout << "  faces: " << poly->n_faces() << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_tetra.plm";
    }

    if (PolyMeshIO::save(out_path, poly.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}