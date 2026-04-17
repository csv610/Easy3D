#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/algo/point_cloud_ransac.h>
#include <memory>
#include <easy3d/renderer/renderer.h>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <args.hxx>

using namespace easy3d;

int main(int argc, char** argv) {
    args::ArgumentParser parser("Plane Extraction - Extract planes from point cloud using RANSAC");

    args::Positional<std::string> input_file(parser, "input", "Input point cloud file");
    args::Positional<std::string> output_file(parser, "output", "Output file for segmented point cloud");
    args::ValueFlag<int> iterations(parser, "iterations", "Number of RANSAC iterations (default: 200)", {'i', "iterations"}, 200);
    args::ValueFlag<float> epsilon(parser, "epsilon", "Epsilon for inlier distance (default: 0.005)", {'e', "epsilon"}, 0.005f);
    args::ValueFlag<float> normal_diff(parser, "normal-diff", "Normal difference threshold (default: 0.02)", {'n', "normal-diff"}, 0.02f);
    args::ValueFlag<float> min_inliers(parser, "min-inliers", "Minimum inliers ratio (default: 0.8)", {'m', "min-inliers"}, 0.8f);
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
        file_path = resource::directory() + "/data/polyhedron.bin";
    }

    std::unique_ptr<PointCloud> cloud(PointCloudIO::load(file_path));
    if (!cloud) {
        LOG(ERROR) << "failed to load point cloud from: " << file_path;
        return EXIT_FAILURE;
    }

    auto normals = cloud->get_vertex_property<vec3>("v:normal");
    if (!normals) {
        std::cerr << "Error: Point cloud has no normal information." << std::endl;
        std::cerr << "Please run normal_estimation first to compute normals." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "point cloud loaded: " << cloud->n_vertices() << " points" << std::endl;

    PrimitivesRansac algo;
    algo.add_primitive_type(PrimitivesRansac::PLANE);

    int iter = args::get(iterations);
    float eps = args::get(epsilon);
    float ndiff = args::get(normal_diff);
    float min_inlier = args::get(min_inliers);

    std::cout << "extracting planes (iterations=" << iter << ", epsilon=" << eps << ")..." << std::endl;

    int num = algo.detect(cloud.get(), iter, eps, ndiff, min_inlier, 0.001f);
    if (num > 0) {
        std::cout << num << " plane(s) extracted" << std::endl;

        auto segments = cloud->vertex_property<int>("v:primitive_index");
        const std::string color_name = "v:color-segments";
        auto coloring = cloud->vertex_property<vec3>(color_name, vec3(0, 0, 0));
        Renderer::color_from_segmentation(cloud.get(), segments, coloring);

        std::string out_path;
        if (output_file) {
            out_path = args::get(output_file);
        } else {
            out_path = file_path.substr(0, file_path.find_last_of('.')) + "_planes.bin";
        }

        if (PointCloudIO::save(out_path, cloud.get())) {
            std::cout << "saved to: " << out_path << std::endl;
        } else {
            std::cerr << "failed to save to: " << out_path << std::endl;
        }
    } else {
        std::cout << "no planes extracted" << std::endl;
    }

    return EXIT_SUCCESS;
}