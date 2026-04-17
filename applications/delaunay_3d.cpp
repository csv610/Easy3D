#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/algo/delaunay_3d.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Delaunay Triangulation 3D");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud file");
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

    const std::vector<vec3>& points = cloud->points();

    std::cout << "computing Delaunay triangulation 3D..." << std::endl;
    Delaunay3 delaunay;
    delaunay.set_vertices(points);

    std::cout << "triangulation result:" << std::endl;
    std::cout << "  vertices: " << delaunay.nb_vertices() << std::endl;
    std::cout << "  tetrahedra: " << delaunay.nb_tets() << std::endl;

    std::cout << "Note: 3D Delaunay mesh is computed in memory. Use other tools to convert to file." << std::endl;
    std::cout << "To save as surface mesh, convert the boundary faces to a SurfaceMesh." << std::endl;

    return EXIT_SUCCESS;
}