#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include "geometry.h"

struct Sphere {
    Vec3f center;
    float radius;
};

bool sphere_ray_intersect(const Sphere &sphere, const Vec3f &orig, const Vec3f &dir, float &t0) {
    Vec3f L = sphere.center - orig;
    float tca = L*dir;
    float d2 = L*L - tca*tca;
    if (d2 > pow(sphere.radius, 2)) return false;
    float thc = sqrtf(pow(sphere.radius, 2) - d2);
    t0       = tca - thc;
    float t1 = tca + thc;
    if (t0 < 0) t0 = t1;
    if (t0 < 0) return false;
    return true;
}

float scene_intersect(const Vec3f &orig, const Vec3f &dir, const std::vector<Sphere> &spheres) {
    float spheres_dist = std::numeric_limits<float>::max();
    for (size_t i=0; i<spheres.size(); i++) {
        float dist_i;
        if (sphere_ray_intersect(spheres[i], orig, dir, dist_i) && dist_i < spheres_dist) {
            spheres_dist = dist_i;
        }
    }

    float checkerboard_dist = std::numeric_limits<float>::max();
    if (fabs(dir.y)>1e-3) {
        float d = -(orig.y+4)/dir.y; // the checkerboard plane has equation y = -4
        Vec3f pt = orig + dir*d;
        if (d>0 && fabs(pt.x)<10 && pt.z<-10 && pt.z>-30) {
            checkerboard_dist = d;
        }
    }

    return std::min(spheres_dist, checkerboard_dist);
}

void compute_depthmap(const size_t width, const size_t height, const float fov, const float far, const std::vector<Sphere> &spheres, std::vector<float> &zbuffer) {
#pragma omp parallel for
    for (size_t j=0; j<height; j++) {
        for (size_t i=0; i<width; i++) {
            float dir_x =  (i + 0.5) - ( width/2.);
            float dir_y = -(j + 0.5) + (height/2.);   // this flips the image at the same time
            float dir_z = -(height/(2.*tan(fov/2.)));
            Vec3f dir = Vec3f{dir_x, dir_y, dir_z}.normalize();
            zbuffer[i+j*width] = scene_intersect(Vec3f{0,0,0}, dir, spheres);
        }
    }

    float minval =  std::numeric_limits<float>::max(); // clamp the zbuffer by the far plane and normalize it between 0 and 1
    float maxval = -std::numeric_limits<float>::max();
    for (size_t i=0; i<height*width; ++i) {
        minval = std::min(minval, zbuffer[i]);
        maxval = std::max(maxval, std::min(zbuffer[i], far));
    }
    for (size_t i=0; i<height*width; ++i)
        zbuffer[i] = 1-(std::min(zbuffer[i],far)-minval)/(maxval-minval);
}

int main() {
    std::vector<Sphere> spheres = { {{-3,0,-16}, 2}, {{-1,-1.5,-12}, 2}, {{1.5,-.5,-18}, 3}, {{7,5,-18}, 4} };
    const size_t width  = 1024;
    const size_t height = 768;
    const float fov  = M_PI/3.;
    std::vector<float> zbuffer(width*height);
    compute_depthmap(width, height, fov, 23.f, spheres, zbuffer);

    std::vector<unsigned char> framebuffer(width*height*3);
    for (size_t j=0; j<height; j++) {
        for (size_t i=0; i<width; i++) {
            for (size_t d=0; d<3; d++) {
                framebuffer[(i+j*width)*3 + d] = 255*zbuffer[i+j*width];
            }
        }
    }

    std::ofstream ofs("./out.ppm"); // save the framebuffer to file
    ofs << "P6\n" << width << " " << height << "\n255\n";
    ofs.write(reinterpret_cast<char *>(framebuffer.data()), 3*height*width);
    ofs.close();

    return 0;
}

