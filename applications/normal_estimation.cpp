#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/algo/point_cloud_normals.h>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>
#include <memory>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Normal Estimation - Estimate normals for a point cloud");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud file");
    args::Positional<std::string> output_file(parser, "output", "Output file for point cloud with normals");
    args::ValueFlag<int> depth(parser, "depth", "Estimation depth (default: 8)", {'d', "depth"}, 8);
    args::ValueFlag<bool> reorient(parser, "reorient", "Reorient normals to be consistent", {'r', "reorient"});
    args::HelpFlag help(parser, "help", "Display this help message", {'h', 'help'});

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
        LOG(ERROR) << "failed to load model from: " << file_path;
        return EXIT_FAILURE;
    }

    std::cout << "point cloud loaded: " << cloud->n_vertices() << " points" << std::endl;

    int d = args::get(depth);
    std::cout << "estimating normals (depth = " << d << ")..." << std::endl;

    if (PointCloudNormals::estimate(cloud.get(), d)) {
        std::cout << "normals estimated successfully" << std::endl;
    } else {
        std::cerr << "failed to estimate normals" << std::endl;
        return EXIT_FAILURE;
    }

    if (args::get(reorient)) {
        std::cout << "reorienting normals..." << std::endl;
        if (PointCloudNormals::reorient(cloud.get())) {
            std::cout << "normals reoriented successfully" << std::endl;
        } else {
            std::cerr << "failed to reorient normals" << std::endl;
        }
    }

    if (output_file) {
        std::string out_path = args::get(output_file);
        if (PointCloudIO::save(out_path, cloud.get())) {
            std::cout << "saved to: " << out_path << std::endl;
        } else {
            std::cerr << "failed to save to: " << out_path << std::endl;
        }
    } else {
        std::string out_path = file_path.substr(0, file_path.find_last_of('.')) + "_normal.bin";
        if (PointCloudIO::save(out_path, cloud.get())) {
            std::cout << "saved to: " << out_path << std::endl;
        }
    }

    return EXIT_SUCCESS;
}