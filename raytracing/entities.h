//
// Created by Виктор Задябин on 09.03.2020.
//

#ifndef TINYRAYTRACER_ENTITIES_H
#define TINYRAYTRACER_ENTITIES_H
namespace raytracing {
    namespace entities {

        struct Light {
            /// Light is a source of light
            /// \param p - position of light source
            /// \param i - intensity of light source
            Light(const Vec3f &p, const float i) : position(p), intensity(i) {}

            Vec3f position;
            float intensity;
        };

        struct Material {
            /// Material is a
            /// \param r
            /// \param a
            /// \param color
            /// \param spec
            Material(const float r, const Vec4f &a, const Vec3f &color, const float spec) : refractive_index(r),
                                                                                            albedo(a),
                                                                                            diffuse_color(color),
                                                                                            specular_exponent(spec) {}

            Material() : refractive_index(1), albedo(1, 0, 0, 0), diffuse_color(), specular_exponent() {}

            float refractive_index;
            Vec4f albedo;
            Vec3f diffuse_color;
            float specular_exponent;
        };

        struct Sphere {
            Vec3f center;
            float radius;
            Material material;

            /// Sphere - object that constructed by
            /// \param c - its center
            /// \param r - its radius
            /// \param m - its material
            Sphere(const Vec3f &c, const float r, const Material &m) : center(c), radius(r), material(m) {}

            /// ray_intersect - checks if ray intersects sphere
            /// \param orig - position of light
            /// \param dir - direction of light ray
            /// \param t0 - coordinate of intersection
            /// \return true if ray intersects sphere, false otherwise
            bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const;
        };

    }// namespace entities
}
#endif //TINYRAYTRACER_ENTITIES_H
