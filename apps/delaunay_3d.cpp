#include <easy3d/core/point_cloud.h>
#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/delaunay_3d.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <easy3d/util/file_system.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Delaunay Triangulation 3D");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud or mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file (default: del3d.off)");
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
        file_path = resource::directory() + "/data/bunny.bin";
    }

    std::vector<vec3> pts;
    std::string input_ext = file_system::extension(file_path, true);

    // Try loading as point cloud first for typical PC formats
    if (input_ext == "ply" || input_ext == "bin" || input_ext == "xyz" || input_ext == "las" || input_ext == "laz") {
        std::unique_ptr<PointCloud> cloud(PointCloudIO::load(file_path));
        if (cloud) {
            std::cout << "point cloud loaded: " << cloud->n_vertices() << " points" << std::endl;
            pts = cloud->points();
        }
    }

    // If not loaded yet, try as surface mesh
    if (pts.empty()) {
        std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
        if (mesh) {
            std::cout << "surface mesh loaded: " << mesh->n_vertices() << " vertices" << std::endl;
            for (auto v : mesh->vertices()) {
                pts.push_back(mesh->position(v));
            }
        }
    }

    if (pts.empty()) {
        LOG(ERROR) << "failed to load points from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "computing Delaunay triangulation 3D..." << std::endl;
    Delaunay3 delaunay;
    delaunay.set_vertices(pts);

    std::cout << "triangulation result:" << std::endl;
    std::cout << "  vertices: " << delaunay.nb_vertices() << std::endl;
    std::cout << "  tetrahedra: " << delaunay.nb_tets() << std::endl;

    std::cout << "extracting boundary (convex hull)..." << std::endl;
    std::unique_ptr<SurfaceMesh> out_mesh(new SurfaceMesh);
    // Add all vertices to the output mesh
    for (unsigned int i = 0; i < delaunay.nb_vertices(); ++i) {
        out_mesh->add_vertex(delaunay.vertex(i));
    }

    // Extract boundary faces: faces of tetrahedra that have no adjacent tetrahedron
    int nb_boundary_faces = 0;
    for (unsigned int t = 0; t < delaunay.nb_tets(); ++t) {
        for (unsigned int f = 0; f < 4; ++f) {
            if (delaunay.tet_adjacent(t, f) < 0) {
                // This facet is on the boundary
                out_mesh->add_triangle(
                    SurfaceMesh::Vertex(delaunay.tet_facet_vertex(t, f, 0)),
                    SurfaceMesh::Vertex(delaunay.tet_facet_vertex(t, f, 1)),
                    SurfaceMesh::Vertex(delaunay.tet_facet_vertex(t, f, 2))
                );
                nb_boundary_faces++;
            }
        }
    }
    std::cout << "  boundary faces: " << nb_boundary_faces << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = "del3d.off";
    }

    if (SurfaceMeshIO::save(out_path, out_mesh.get())) {
        std::cout << "saved boundary to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save boundary to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}