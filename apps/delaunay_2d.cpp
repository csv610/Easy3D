#include <easy3d/core/point_cloud.h>
#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/delaunay_2d.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Delaunay Triangulation 2D");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud file");
    args::Positional<std::string> output_file(parser, "output", "Output mesh file");
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

    std::unique_ptr<PointCloud> cloud(PointCloudIO::load(file_path));
    if (!cloud) {
        LOG(ERROR) << "failed to load point cloud from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "point cloud loaded: " << cloud->n_vertices() << " points" << std::endl;

    auto points = cloud->get_vertex_property<vec3>("v:point");
    std::vector<vec2> pts;
    for (auto v : cloud->vertices()) {
        pts.push_back(vec2(points[v].x, points[v].y));
    }

    std::cout << "computing Delaunay triangulation 2D..." << std::endl;
    Delaunay2 delaunay;
    delaunay.set_vertices(pts);

    std::cout << "triangulation result:" << std::endl;
    std::cout << "  vertices: " << delaunay.nb_vertices() << std::endl;
    std::cout << "  triangles: " << delaunay.nb_triangles() << std::endl;

    std::unique_ptr<SurfaceMesh> mesh(new SurfaceMesh);
    for (unsigned int i = 0; i < delaunay.nb_vertices(); ++i) {
        mesh->add_vertex(vec3(delaunay.vertex(i).x, delaunay.vertex(i).y, 0.0f));
    }

    const int* tri_to_v = delaunay.tri_to_v();
    for (unsigned int i = 0; i < delaunay.nb_triangles(); ++i) {
        mesh->add_triangle(
            SurfaceMesh::Vertex(tri_to_v[3 * i]),
            SurfaceMesh::Vertex(tri_to_v[3 * i + 1]),
            SurfaceMesh::Vertex(tri_to_v[3 * i + 2])
        );
    }

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_delaunay_2d.obj";
    }

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}