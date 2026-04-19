#include <easy3d/algo/delaunay_2d.h>
#include <easy3d/algo/delaunay_3d.h>
#include <easy3d/util/initializer.h>
#include <iostream>
#include <vector>

using namespace easy3d;

int main() {
    initialize();

    std::cout << "Testing Delaunay2 with duplicates..." << std::endl;
    {
        std::vector<vec2> points = {
            {0, 0}, {1, 0}, {0, 1}, {1, 1},
            {0, 0}, // Duplicate
            {0.5, 0.5}
        };
        Delaunay2 del2;
        del2.set_vertices(points);
        std::cout << "  Input points: " << points.size() << std::endl;
        std::cout << "  Delaunay2 vertices: " << del2.nb_vertices() << std::endl;
        std::cout << "  Delaunay2 triangles: " << del2.nb_cells() << std::endl;
    }

    std::cout << "\nTesting Delaunay3 with duplicates..." << std::endl;
    {
        std::vector<vec3> points = {
            {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
            {0, 0, 0}, // Duplicate
            {0.2, 0.2, 0.2}
        };
        Delaunay3 del3;
        del3.set_vertices(points);
        std::cout << "  Input points: " << points.size() << std::endl;
        std::cout << "  Delaunay3 vertices: " << del3.nb_vertices() << std::endl;
        std::cout << "  Delaunay3 tetrahedra: " << del3.nb_tets() << std::endl;
    }

    return 0;
}
