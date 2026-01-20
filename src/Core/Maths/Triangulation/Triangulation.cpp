#include "Triangulation.h"
#include "../Vector3.h"
#include "../Vector2.hpp"
#include "../Matrix.h"
#include "delaunay.hpp"

namespace gce
{
    std::vector<Vector2f32> ProjectToPlane(std::vector<Vector3f32> const& points)
    {

        Vector3f32 centroid = { 0, 0, 0 };
        for (Vector3f32 p : points)
            centroid = centroid + p;
        centroid = centroid / float32(points.size());

        Vector3f32 xAxis = (points[1] - points[0]).Normalize();
        Vector3f32 temp = (points[2] - points[0]).Normalize();

        Vector3f32 zAxis = xAxis.CrossProduct(temp).Normalize();
        Vector3f32 yAxis = zAxis.CrossProduct(xAxis).Normalize();

        std::vector<Vector2f32> projected;
        for (Vector3f32 p : points)
        {
            Vector3f32 local = p - centroid;
            float32 x = local.DotProduct(xAxis);
            float32 y = local.DotProduct(yAxis);
            projected.push_back({ x, y });
        }

        return projected;
    }

    std::vector<uint32> Triangulate(std::vector<Vector2f32> const& pos)
    {
        std::vector<uint32> indices;
        Tessellator meshIndices(pos);

        for (Triangle tri : meshIndices.GetTriangles())
        {
            indices.push_back(tri.a_);
            indices.push_back(tri.b_);
            indices.push_back(tri.c_);
        }

        return indices;
    }

}