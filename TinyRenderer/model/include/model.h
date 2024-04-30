#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "Eigen/Core"

class Model {
private:
    std::vector<std::vector<int> > faces_;
    std::vector<Eigen::Vector3d> verts_;

    std::vector<std::vector<int>> uvIndexes_;
    std::vector<Eigen::Vector2d> uvs_;

    std::vector<std::vector<int>> normalIndexes_;
    std::vector<Eigen::Vector3d> normals_;
public:
    explicit Model(const char *filename);

    ~Model();

    int nFaces();

    int nVerts();

    std::vector<int> face(int idx);

    Eigen::Vector3d vert(int i);


    int nUVIndexes();

    int nUVs();

    std::vector<int> uvIndex(int i);

    Eigen::Vector2d uv(int i);


    int nNormalIndexes();

    int nNormals();

    std::vector<int> normalIndex(int i);

    Eigen::Vector3d normal(int i);
};

#endif //__MODEL_H__