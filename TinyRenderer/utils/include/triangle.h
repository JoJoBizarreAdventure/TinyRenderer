#ifndef TINY_RENDERER_TRIANGLE_H
#define TINY_RENDERER_TRIANGLE_H

#include "tgaimage.h"
#include "functional"
#include "Eigen/Core"

void triangle_l2(Eigen::Vector2i &t0, Eigen::Vector2i &t1, Eigen::Vector2i &t2, TGAImage &image, const TGAColor &color);

void triangle_l3(Eigen::Vector3d t0, Eigen::Vector3d t1, Eigen::Vector3d t2, TGAImage &image, double *zBuffer,
                 const std::function<TGAColor(double, double)> &getColor);


#endif //TINY_RENDERER_TRIANGLE_H
