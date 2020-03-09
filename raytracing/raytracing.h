#ifndef TINYRAYTRACER_RAYTRACING_H
#define TINYRAYTRACER_RAYTRACING_H

#include "../geometry.h"
#include "entities.h"
namespace raytracing {

/// render - function for filling image with blue color, also for adding spheres and lights to
///          picture,
/// \param spheres - vector of spheres
/// \param lights - vector of lights
void render(const std::vector<entities::Sphere> &spheres, const std::vector<entities::Light> &lights);

}//namespace raytracing

#endif //TINYRAYTRACER_RAYTRACING_H
