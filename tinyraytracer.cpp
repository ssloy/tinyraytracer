#include <limits>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry.h"

struct Light {
    vec3 position;
    float intensity;
};

struct Material {
    float refractive_index = 1;
    vec4 albedo = {1,0,0,0};
    vec3 diffuse_color = {0,0,0};
    float specular_exponent = 0;
};

struct Sphere {
    vec3 center;
    float radius;
    Material material;
};

static const Material      ivory = {1.0, {0.6,  0.3, 0.1, 0.0}, {0.4, 0.4, 0.3},   50.};
static const Material      glass = {1.5, {0.0,  0.5, 0.1, 0.8}, {0.6, 0.7, 0.8},  125.};
static const Material red_rubber = {1.0, {0.9,  0.1, 0.0, 0.0}, {0.3, 0.1, 0.1},   10.};
static const Material     mirror = {1.0, {0.0, 10.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425.};

static const std::vector<Sphere> spheres = {
    Sphere{vec3{-3,    0,   -16}, 2,      ivory},
    Sphere{vec3{-1.0, -1.5, -12}, 2,      glass},
    Sphere{vec3{ 1.5, -0.5, -18}, 3, red_rubber},
    Sphere{vec3{ 7,    5,   -18}, 4,     mirror}
};

static const std::vector<Light> lights = {
    {{-20, 20,  20}, 1.5},
    {{ 30, 50, -25}, 1.8},
    {{ 30, 20,  30}, 1.7}
};

bool ray_sphere_intersect(const vec3 &orig, const vec3 &dir, const Sphere &s, float &t0) {
    vec3 L = s.center - orig;
    float tca = L*dir;
    float d2 = L*L - tca*tca;
    if (d2 > s.radius*s.radius) return false;
    float thc = std::sqrt(s.radius*s.radius - d2);
    t0       = tca - thc;
    float t1 = tca + thc;
    if (t0 < 1e-3) t0 = t1;  // offset the original point to avoid occlusion by the object itself
    if (t0 < 1e-3) return false;
    return true;
}

vec3 reflect(const vec3 &I, const vec3 &N) {
    return I - N*2.f*(I*N);
}

vec3 refract(const vec3 &I, const vec3 &N, const float eta_t, const float eta_i=1.f) { // Snell's law
    float cosi = - std::max(-1.f, std::min(1.f, I*N));
    if (cosi<0) return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
    float eta = eta_i / eta_t;
    float k = 1 - eta*eta*(1 - cosi*cosi);
    return k<0 ? vec3{1,0,0} : I*eta + N*(eta*cosi - std::sqrt(k)); // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}

bool scene_intersect(const vec3 &orig, const vec3 &dir, vec3 &hit, vec3 &N, Material &material) {
    float spheres_dist = std::numeric_limits<float>::max();
    for (const Sphere &s : spheres) {
        float dist_i;
        if (ray_sphere_intersect(orig, dir, s, dist_i) && dist_i < spheres_dist) {
            spheres_dist = dist_i;
            hit = orig + dir*dist_i;
            N = (hit - s.center).normalize();
            material = s.material;
        }
    }

    float checkerboard_dist = std::numeric_limits<float>::max();
    if (std::abs(dir.y)>1e-3) { // avoid division by zero
        float d = -(orig.y+4)/dir.y; // the checkerboard plane has equation y = -4
        vec3 pt = orig + dir*d;
        if (d>1e-3 && fabs(pt.x)<10 && pt.z<-10 && pt.z>-30 && d<spheres_dist) {
            checkerboard_dist = d;
            hit = pt;
            N = vec3{0,1,0};
            material.diffuse_color = (int(.5*hit.x+1000) + int(.5*hit.z)) & 1 ? vec3{.3, .3, .3} : vec3{.3, .2, .1};
        }
    }
    return std::min(spheres_dist, checkerboard_dist)<1000;
}

vec3 cast_ray(const vec3 &orig, const vec3 &dir, size_t depth=0) {
    vec3 point, N;
    Material material;

    if (depth>4 || !scene_intersect(orig, dir, point, N, material))
        return vec3{0.2, 0.7, 0.8}; // background color

    vec3 reflect_dir = reflect(dir, N).normalize();
    vec3 refract_dir = refract(dir, N, material.refractive_index).normalize();
    vec3 reflect_color = cast_ray(point, reflect_dir, depth + 1);
    vec3 refract_color = cast_ray(point, refract_dir, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (const Light light : lights) {
        vec3 light_dir = (light.position - point).normalize();

        vec3 shadow_pt, trashnrm;
        Material trashmat;
        if (scene_intersect(point, light_dir, shadow_pt, trashnrm, trashmat) &&
                (shadow_pt-point).norm() < (light.position-point).norm()) // checking if the point lies in the shadow of the light
            continue;

        diffuse_light_intensity  += light.intensity * std::max(0.f, light_dir*N);
        specular_light_intensity += std::pow(std::max(0.f, -reflect(-light_dir, N)*dir), material.specular_exponent)*light.intensity;
    }
    return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + vec3{1., 1., 1.}*specular_light_intensity * material.albedo[1] + reflect_color*material.albedo[2] + refract_color*material.albedo[3];
}

int main() {
    const int   width  = 1024;
    const int   height = 768;
    const float fov    = M_PI/3.;
    std::vector<vec3> framebuffer(width*height);
#pragma omp parallel for
    for (size_t j = 0; j<height; j++) { // actual rendering loop
        for (size_t i = 0; i<width; i++) {
            float dir_x =  (i + 0.5) -  width/2.;
            float dir_y = -(j + 0.5) + height/2.;    // this flips the image at the same time
            float dir_z = -height/(2.*tan(fov/2.));
            framebuffer[i+j*width] = cast_ray(vec3{0,0,0}, vec3{dir_x, dir_y, dir_z}.normalize());
        }
    }

    std::ofstream ofs; // save the framebuffer to file
    ofs.open("./out.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (vec3 &c : framebuffer) {
        float max = std::max(c[0], std::max(c[1], c[2]));
        if (max>1) c = c*(1./max);
        ofs << (char)(255 * c[0]) << (char)(255 * c[1]) << (char)(255 * c[2]);
    }
    ofs.close();
    return 0;
}

