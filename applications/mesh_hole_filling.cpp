#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/surface_mesh_hole_filling.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Mesh Hole Filling");

    args::Positional<std::string> input_file(parser, "input", "Input surface mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file");
    args::ValueFlag<int> max_size(parser, "max-size", "Maximum boundary size to fill (default: 500)", {'m', "max-size"}, 500);
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

    int max_boundary = args::get(max_size);

    std::vector<std::pair<SurfaceMesh::Halfedge, int>> holes;

    auto visited = mesh->add_halfedge_property<bool>("h:visited", false);
    for (auto h : mesh->halfedges()) {
        if (!visited[h] && mesh->is_border(h)) {
            int size = 0;
            SurfaceMesh::Halfedge hh = h;
            do {
                visited[hh] = true;
                ++size;
                if (!mesh->is_manifold(mesh->target(hh))) {
                    size += 123456;
                    break;
                }
                hh = mesh->next(hh);
            } while (hh != h);

            if (size < max_boundary) {
                holes.push_back({h, size});
            }
        }
    }
    mesh->remove_halfedge_property(visited);

    std::cout << "found " << holes.size() << " holes with boundary size < " << max_boundary << std::endl;

    int num_filled = 0;
    for (const auto& hole : holes) {
        SurfaceMeshHoleFilling hf(mesh.get());
        hf.fill_hole(hole.first);
        ++num_filled;
    }

    std::cout << "filled " << num_filled << " holes" << std::endl;
    std::cout << "result: " << mesh->n_vertices() << " vertices, " << mesh->n_faces() << " faces" << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_filled.ply";
    }

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}