#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> verts;
    std::vector<Vec3i> faces;
public:
    Model(const char *filename);

    int nverts() const;                          // number of vertices
    int nfaces() const;                          // number of triangles

    bool ray_triangle_intersect(const int &fi, const Vec3f &orig, const Vec3f &dir, float &tnear);

    const Vec3f &point(int i) const;                   // coordinates of the vertex i
    Vec3f &point(int i);                   // coordinates of the vertex i
    int vert(int fi, int li) const;              // index of the vertex for the triangle fi and local index li
    void get_bbox(Vec3f &min, Vec3f &max); // bounding box for all the vertices, including isolated ones
};

std::ostream& operator<<(std::ostream& out, Model &m);

#endif //__MODEL_H__

