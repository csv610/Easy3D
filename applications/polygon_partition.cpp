#include <easy3d/core/surface_mesh.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/algo/polygon_partition.h>
#include <memory>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Polygon Partition - Partition a polygon with holes into convex parts");

    args::Positional<std::string> output_file(parser, "output", "Output mesh file with convex polygons");
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

    std::vector<vec2> points = {
        vec2(0, 0), vec2(500, 0), vec2(500, 700), vec2(0, 700),
        vec2(100, 100), vec2(400, 100), vec2(400, 300), vec2(100, 300),
        vec2(100, 400), vec2(400, 400), vec2(400, 600), vec2(100, 600),
    };

    std::vector<PolygonPartition::Polygon> polygons = {{0, 1, 2, 3}};
    std::vector<PolygonPartition::Polygon> holes = {{7, 6, 5, 4}, {11, 10, 9, 8}};

    std::cout << "input polygon has 1 outer contour and 2 holes" << std::endl;
    std::cout << "partitioning into convex polygons..." << std::endl;

    std::vector<PolygonPartition::Polygon> parts;
    if (!PolygonPartition::apply(points, polygons, holes, parts)) {
        std::cerr << "partition failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "partitioned into " << parts.size() << " convex polygons" << std::endl;

    std::unique_ptr<SurfaceMesh> mesh(new SurfaceMesh);
    for (const auto& p : points)
        mesh->add_vertex({p.x, p.y, 0});
    for (const auto& poly : parts) {
        std::vector<SurfaceMesh::Vertex> vts;
        for (const auto& id : poly)
            vts.push_back(SurfaceMesh::Vertex(static_cast<int>(id)));
        mesh->add_face(vts);
    }

    std::string out_path = output_file ? args::get(output_file) : "./polygon_partition.obj";

    if (SurfaceMeshIO::save(out_path, mesh.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}