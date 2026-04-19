#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

#if HAS_CGAL
#include <easy3d/algo_ext/self_intersection.h>
#endif

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Self-Intersection Detection (requires CGAL)");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file with intersecting faces marked");
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

#if !HAS_CGAL
    std::cerr << "Error: CGAL is not enabled. Please rebuild with CGAL support." << std::endl;
    return EXIT_FAILURE;
#endif

    initialize();

    std::string file_path;
    if (input_file) {
        file_path = args::get(input_file);
    } else {
        file_path = resource::directory() + "/data/house/house.obj";
    }

#if HAS_CGAL
    SurfaceMesh* mesh = SurfaceMeshIO::load(file_path);
    if (!mesh) {
        LOG(ERROR) << "failed to load mesh from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "mesh loaded: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    std::cout << "detecting self-intersections..." << std::endl;

    auto result = SelfIntersection::detect(mesh);

    std::cout << "detection complete: " << result.intersecting_pairs.size()
              << " pairs of intersecting triangles found" << std::endl;

    if (!result.intersecting_pairs.empty()) {
        auto intersecting = mesh->add_face_property<bool>("f:self_intersecting", false);
        for (const auto& pair : result.intersecting_pairs) {
            intersecting[SurfaceMesh::Face(pair.first)] = true;
            intersecting[SurfaceMesh::Face(pair.second)] = true;
        }
    }

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_intersections.ply";
    }

    if (SurfaceMeshIO::save(out_path, mesh)) {
        std::cout << "saved to: " << out_path << std::endl;
        std::cout << "intersecting faces marked in face property 'f:self_intersecting'" << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    delete mesh;
#endif
    return EXIT_SUCCESS;
}