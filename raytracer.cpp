#include "raytracing/raytracing.h"

int main() {
    try {
        raytracing::entities::Material ivory(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3), 50.);
        raytracing::entities::Material glass(1.5, Vec4f(0.0, 0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8), 125.);
        raytracing::entities::Material red_rubber(1.0, Vec4f(0.9, 0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1), 10.);
        raytracing::entities::Material mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

        std::vector<raytracing::entities::Sphere> spheres;
        spheres.push_back(raytracing::entities::Sphere(Vec3f(-3, 0, -16), 2, ivory));
        spheres.push_back(raytracing::entities::Sphere(Vec3f(-1.0, -1.5, -12), 2, glass));
        spheres.push_back(raytracing::entities::Sphere(Vec3f(1.5, -0.5, -18), 3, red_rubber));
        spheres.push_back(raytracing::entities::Sphere(Vec3f(7, 5, -18), 4, mirror));

        std::vector<raytracing::entities::Light> lights;
        lights.push_back(raytracing::entities::Light(Vec3f(-20, 20, 20), 1.5));
        lights.push_back(raytracing::entities::Light(Vec3f(30, 50, -25), 1.8));
        lights.push_back(raytracing::entities::Light(Vec3f(30, 20, 30), 1.7));

        raytracing::render(spheres, lights);

        std::cout << "result is saved in build directory in file out.ppm" << std::endl;
    }
    catch (const std::exception& er){
        std::cout << er.what();
    }
    return 0;
}

