#ifndef TINY_RENDERER_LINE_H
#define TINY_RENDERER_LINE_H

#include "tgaimage.h"
#include "Eigen/Core"

void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color);

void line(Eigen::Vector2i t0, Eigen::Vector2i t1, TGAImage &image, const TGAColor &color);

#endif //TINY_RENDERER_LINE_H
