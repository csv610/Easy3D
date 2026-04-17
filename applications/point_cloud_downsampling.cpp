#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/algo/point_cloud_simplification.h>
#include <memory>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Point Cloud Downsampling");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud file");
    args::Positional<std::string> output_file(parser, "output", "Output file for downsampled point cloud");
    args::ValueFlag<std::string> method(parser, "method", "Downsampling method: grid, uniform, clustering (default: grid)", {'m', "method"}, "grid");
    args::ValueFlag<float> threshold(parser, "threshold", "Distance threshold (default: 0.01)", {'t', "threshold"}, 0.01f);
    args::ValueFlag<int> num_points(parser, "num", "Target number of points (for uniform method)", {'n', "num"}, 0);
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

    std::string method_name = args::get(method);
    float thresh = args::get(threshold);
    int target_num = args::get(num_points);

    std::vector<PointCloud::Vertex> points_to_remove;

    if (method_name == "grid") {
        std::cout << "grid simplification (cell_size = " << thresh << ")..." << std::endl;
        points_to_remove = PointCloudSimplification::grid_simplification(cloud.get(), thresh);
    } else if (method_name == "uniform") {
        std::cout << "uniform simplification (epsilon = " << thresh << ")..." << std::endl;
        points_to_remove = PointCloudSimplification::uniform_simplification(cloud.get(), thresh);
    } else if (method_name == "num" && target_num > 0) {
        std::cout << "uniform simplification to " << target_num << " points..." << std::endl;
        points_to_remove = PointCloudSimplification::uniform_simplification(cloud.get(), static_cast<unsigned int>(target_num));
    } else {
        std::cout << "grid simplification (cell_size = " << thresh << ")..." << std::endl;
        points_to_remove = PointCloudSimplification::grid_simplification(cloud.get(), thresh);
    }

    for (auto v : points_to_remove)
        cloud->delete_vertex(v);
    cloud->collect_garbage();

    std::cout << "downsampled to: " << cloud->n_vertices() << " points" << std::endl;

    std::string out_path;
    if (output_file) {
        out_path = args::get(output_file);
    } else {
        out_path = file_path.substr(0, file_path.find_last_of('.')) + "_downsampled.bin";
    }

    if (PointCloudIO::save(out_path, cloud.get())) {
        std::cout << "saved to: " << out_path << std::endl;
    } else {
        std::cerr << "failed to save to: " << out_path << std::endl;
    }

    return EXIT_SUCCESS;
}