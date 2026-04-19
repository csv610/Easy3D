#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>
#include <memory>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Point Cloud IO - Load and save point cloud files");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud file");
    args::Positional<std::string> output_file(parser, "output", "Output file (optional)");
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

    if (output_file) {
        std::string out_path = args::get(output_file);
        if (PointCloudIO::save(out_path, cloud.get())) {
            std::cout << "saved to: " << out_path << std::endl;
        } else {
            std::cerr << "failed to save to: " << out_path << std::endl;
        }
    }

    return EXIT_SUCCESS;
}