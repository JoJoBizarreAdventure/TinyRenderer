#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line);
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Eigen::Vector3d v;
            for (int i = 0; i < 3; i++) {
                iss >> v[i];
            }
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            std::vector<int> uvIdxes;
            std::vector<int> normalIdxes;
            int idx, uvIdx, normalIdx;
            iss >> trash;
            while (iss >> idx >> trash >> uvIdx >> trash >> normalIdx) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
                uvIdxes.push_back(uvIdx - 1);
                normalIdxes.push_back(normalIdx - 1);
            }
            faces_.push_back(f);
            uvIndexes_.push_back(uvIdxes);
            normalIndexes_.push_back(normalIdxes);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            int itrash;
            Eigen::Vector2d v;
            for (int i = 0; i < 2; i++) {
                iss >> v[i];
            }
            iss >> itrash;
            uvs_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Eigen::Vector3d v;
            for (int i = 0; i < 3; i++) {
                iss >> v[i];
            }
            normals_.push_back(v);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() = default;

int Model::nFaces() {
    return (int) faces_.size();
}

int Model::nVerts() {
    return (int) verts_.size();
}


std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Eigen::Vector3d Model::vert(int i) {
    return verts_[i];
}

int Model::nUVIndexes() {
    return (int) uvIndexes_.size();
}

int Model::nUVs() {
    return (int) uvs_.size();
}


std::vector<int> Model::uvIndex(int i) {
    return uvIndexes_[i];
}

Eigen::Vector2d Model::uv(int i) {
    return uvs_[i];
}

int Model::nNormalIndexes() {
    return (int) normalIndexes_.size();
}

int Model::nNormals() {
    return (int) normals_.size();
}

std::vector<int> Model::normalIndex(int i) {
    return normalIndexes_[i];
}

Eigen::Vector3d Model::normal(int i) {
    return normals_[i];
}
