#include <easy3d/core/point_cloud.h>
#include <easy3d/core/surface_mesh.h>
#include <easy3d/core/graph.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/fileio/graph_io.h>
#include <easy3d/algo/delaunay_2d.h>
#include <easy3d/algo/point_cloud_simplification.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <easy3d/util/file_system.h>
#include <args.hxx>

using namespace easy3d;

// Helper to compute circumcenter of a 2D triangle
vec2 circum_center(const vec2& p1, const vec2& p2, const vec2& p3) {
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;
    float x3 = p3.x, y3 = p3.y;
    float D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    float ux = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
    float uy = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;
    return vec2(ux, uy);
}

int main(int argc, char** argv) {
    args::ArgumentParser parser("Voronoi Diagram 2D");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud or mesh file");
    args::Positional<std::string> output_file(parser, "output", "Output graph file (default: voronoi_2d.ply)");
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

    std::unique_ptr<PointCloud> cloud;
    std::string input_ext = file_system::extension(file_path, true);
    
    if (input_ext == "ply" || input_ext == "bin" || input_ext == "xyz" || input_ext == "las" || input_ext == "laz") {
        cloud.reset(PointCloudIO::load(file_path));
    } 
    
    if (!cloud) {
        std::unique_ptr<SurfaceMesh> mesh(SurfaceMeshIO::load(file_path));
        if (mesh) {
            cloud = std::make_unique<PointCloud>();
            for (auto v : mesh->vertices()) {
                cloud->add_vertex(mesh->position(v));
            }
        }
    }

    if (!cloud || cloud->n_vertices() == 0) {
        LOG(ERROR) << "failed to load points from: " << file_path;
        return EXIT_FAILURE;
    }

    // Remove duplicates
    auto to_delete = PointCloudSimplification::uniform_simplification(cloud.get(), 1e-6f);
    if (!to_delete.empty()) {
        cloud->delete_vertices(to_delete);
        cloud->collect_garbage();
    }

    std::vector<vec2> pts;
    auto points_prop = cloud->get_vertex_property<vec3>("v:point");
    for (auto v : cloud->vertices()) {
        pts.push_back(vec2(points_prop[v].x, points_prop[v].y));
    }

    std::cout << "computing Delaunay triangulation 2D for Voronoi dual..." << std::endl;
    Delaunay2 delaunay;
    delaunay.set_vertices(pts);

    std::cout << "constructing Voronoi diagram (finite edges)..." << std::endl;
    auto voronoi_graph = std::make_unique<Graph>();
    
    // 1. Compute circumcenters (Voronoi vertices)
    std::vector<Graph::Vertex> voronoi_vertices;
    for (unsigned int i = 0; i < delaunay.nb_triangles(); ++i) {
        vec2 c = circum_center(
            delaunay.vertex(delaunay.tri_vertex(i, 0)),
            delaunay.vertex(delaunay.tri_vertex(i, 1)),
            delaunay.vertex(delaunay.tri_vertex(i, 2))
        );
        voronoi_vertices.push_back(voronoi_graph->add_vertex(vec3(c.x, c.y, 0.0f)));
    }

    // 2. Connect circumcenters of adjacent triangles (Voronoi edges)
    for (unsigned int i = 0; i < delaunay.nb_triangles(); ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            int adj_tri = delaunay.tri_adjacent(i, j);
            if (adj_tri >= 0 && (unsigned int)adj_tri > i) {
                voronoi_graph->add_edge(voronoi_vertices[i], voronoi_vertices[adj_tri]);
            }
        }
    }

    std::string out_path = output_file ? args::get(output_file) : "voronoi_2d.ply";
    if (GraphIO::save(out_path, voronoi_graph.get())) {
        std::cout << "Voronoi diagram (finite edges) saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}
