#include "triangle.h"

#include <utility>
#include "line.h"
#include "vector"
#include "algorithm"

void lineSweeping_myImplement(Eigen::Vector2i t0, Eigen::Vector2i t1, Eigen::Vector2i t2,
                              TGAImage &image, const TGAColor &color) {
    std::vector<Eigen::Vector2i> t = {std::move(t0), std::move(t1), std::move(t2)};
    const auto compareY = [](Eigen::Vector2i v0, Eigen::Vector2i v1) {
        return v0.y() < v1.y();
    };
    sort(t.begin(), t.end(), compareY);
    bool inverse = false; // get mirror triangle => y0=y1<y2 -> y2<y0=y1
    auto drawLine = [&inverse, &image, &color](int x0, int x1, int y) {
        y = inverse ? -y : y;
        line(x0, y, x1, y, image, color);
    };
    auto drawHalfTriangle = [&image, &color, &drawLine, &inverse](Eigen::Vector2i left, Eigen::Vector2i right,
                                                                  Eigen::Vector2i from, int to) {
        if (right.x() < left.x())
            std::swap(left, right);

        float tanAlpha = static_cast<float >(left.x()) / static_cast<float >(left.y());
        float tanBeta = static_cast<float >(right.x()) / static_cast<float >(right.y());
        image.set(from.x(), inverse ? -from.y() : from.y(), color);
        for (int h = 1; h <= to - from.y(); h++) {
            int startX = from.x() + static_cast<int>(tanAlpha * static_cast<float >(h));
            int endX = from.x() + static_cast<int>(tanBeta * static_cast<float >(h));
            int y = from.y() + h;
            drawLine(startX, endX, y);
        }
    };

    if (t[0].y() == t[1].y()) {
        inverse = true;
        std::reverse(t.begin(), t.end());
        for (auto &vec: t) {
            vec.y() = -vec.y();
        }
    }

    Eigen::Vector2i t01 = t[1] - t[0];
    Eigen::Vector2i t02 = t[2] - t[0];

    drawHalfTriangle(t01, t02, t[0], t[1].y());

    if (t[1].y() == t[2].y())
        return;

    Eigen::Vector2i t21 = t[1] - t[2];
    t21.y() = -t21.y();
    Eigen::Vector2i t20 = t[0] - t[2];
    t20.y() = -t20.y();
    inverse = true;
    t[2].y() = -t[2].y();
    drawHalfTriangle(t21, t20, t[2], -t[1].y());
}

void lineSweeping_lessonImplement(Eigen::Vector2i t0, Eigen::Vector2i t1, Eigen::Vector2i t2, TGAImage &image,
                                  const TGAColor &color) {
    if (t0.y() == t1.y() && t0.y() == t2.y()) return; // I dont care about degenerate triangles
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!)
    if (t0.y() > t1.y()) std::swap(t0, t1);
    if (t0.y() > t2.y()) std::swap(t0, t2);
    if (t1.y() > t2.y()) std::swap(t1, t2);
    int total_height = t2.y() - t0.y();
    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y() - t0.y() || t1.y() == t0.y();
        int segment_height = second_half ? t2.y() - t1.y() : t1.y() - t0.y();
        float alpha = (float) i / total_height;
        float beta = (float) (i - (second_half ? t1.y() - t0.y() : 0)) /
                     segment_height; // be careful: with above conditions no division by zero here
        Eigen::Vector2i tA = t2 - t0;
        Eigen::Vector2i tB = second_half ? t2 - t1 : t1 - t0;
        Eigen::Vector2i A = {t0.x() + static_cast<int>(tA.x() * alpha), t0.y() + static_cast<int>(tA.y() * alpha)};
        Eigen::Vector2i B = {(second_half ? t1.x() : t0.x()) + static_cast<int>(tB.x() * beta),
                             (second_half ? t1.y() : t0.y()) + static_cast<int>(tB.y() * beta)};
        if (A.x() > B.x()) std::swap(A, B);
        for (int j = A.x(); j <= B.x(); j++) {
            image.set(j, t0.y() + i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

void boundingBox_myImplement(Eigen::Vector2i t0, Eigen::Vector2i t1, Eigen::Vector2i t2, TGAImage &image,
                             const TGAColor &color) {
    auto cross = [](const Eigen::Vector2i &v0, const Eigen::Vector2i &v1) {
        return v0.x() * v1.y() - v1.x() * v0.y();
    };

    auto isATriangle = [&t0, &t1, &t2, &cross]() {
        Eigen::Vector2i t01 = t1 - t0, t02 = t2 - t0;
        return cross(t01, t02) != 0;
    };
    if (!isATriangle()) {
        return;
    }

    auto isInTriangle1 = [&t0, &t1, &t2, &cross](int x, int y) {
        const Eigen::Vector2i t01 = t1 - t0, t12 = t2 - t1, t20 = t0 - t2;
        Eigen::Vector2i p = {x, y};
        Eigen::Vector2i t0p = p - t0, t1p = p - t1, t2p = p - t2;
        int c0 = cross(t0p, t01), c1 = cross(t1p, t12), c2 = cross(t2p, t20);
        return (c0 >= 0 && c1 >= 0 && c2 >= 0) || (c0 <= 0 && c1 <= 0 && c2 <= 0);
    };
    auto isInTriangle2 = [&t0, &t1, &t2](int x, int y) { //adopt from barycentric
        Eigen::Vector3d vx = {static_cast<float>(t1.x() - t0.x()), static_cast<float>(t2.x() - t0.x()),
                              static_cast<float>(t0.x() - x)};
        Eigen::Vector3d vy = {static_cast<float>(t1.y() - t0.y()), static_cast<float>(t2.y() - t0.y()),
                              static_cast<float>(t0.y() - y)};
        Eigen::Vector3d uv1 = {vx.y() * vy.z() - vx.z() * vy.y(), vx.z() * vy.x() - vx.x() * vy.z(),
                               vx.x() * vy.y() - vx.y() * vy.x()};
        if (std::abs(uv1.z()) < 1)
            return false;
        float u = uv1.x() / uv1.z(), v = uv1.y() / uv1.z();
        return u >= 0 && v >= 0 && u + v <= 1;
    };
    int fromX = std::min(t0.x(), std::min(t1.x(), t2.x())),
            toX = std::max(t0.x(), std::max(t1.x(), t2.x())),
            fromY = std::min(t0.y(), std::min(t1.y(), t2.y())),
            toY = std::max(t0.y(), std::max(t1.y(), t2.y()));

    for (int x = fromX; x <= toX; x++) {
        for (int y = fromY; y <= toY; y++) {
            if (isInTriangle1(x, y)) {
                image.set(x, y, color);
            }
        }
    }
}

void boundingBox_lessonImplement(Eigen::Vector2i t0, Eigen::Vector2i t1, Eigen::Vector2i t2, TGAImage &image,
                                 const TGAColor &color) {
    Eigen::Vector2i bboxmin(image.get_width() - 1, image.get_height() - 1);
    Eigen::Vector2i bboxmax(0, 0);
    Eigen::Vector2i clamp(image.get_width() - 1, image.get_height() - 1);
    std::vector<Eigen::Vector2i> pts = {std::move(t0), std::move(t1), std::move(t2)};
    for (int i = 0; i < 3; i++) {
        bboxmin.x() = std::max(0, std::min(bboxmin.x(), pts[i].x()));
        bboxmin.y() = std::max(0, std::min(bboxmin.y(), pts[i].y()));

        bboxmax.x() = std::min(clamp.x(), std::max(bboxmax.x(), pts[i].x()));
        bboxmax.y() = std::min(clamp.y(), std::max(bboxmax.y(), pts[i].y()));
    }
    auto barycentric = [](std::vector<Eigen::Vector2i> pts, Eigen::Vector2i P) {
        Eigen::Vector3d vx = Eigen::Vector3d(pts[2].x() - pts[0].x(), pts[1].x() - pts[0].x(), pts[0].x() - P.x());
        Eigen::Vector3d vy = Eigen::Vector3d(pts[2].y() - pts[0].y(), pts[1].y() - pts[0].y(), pts[0].y() - P.y());
        Eigen::Vector3d u = {vx.y() * vy.z() - vx.z() * vy.y(), vx.z() * vy.x() - vx.x() * vy.z(),
                             vx.x() * vy.y() - vx.y() * vy.x()};
        /* `pts` and `P` has integer value as coordinates
           so `abs(u[2])` < 1 means `u[2]` is 0, that means
           triangle is degenerate, in this case return something with negative coordinates */
        if (std::abs(u.z()) < 1) return Eigen::Vector3d(-1, 1, 1);
        return Eigen::Vector3d(1.f - (u.x() + u.y()) / u.z(), u.y() / u.z(), u.x() / u.z());
    };
    Eigen::Vector2i P;
    for (P.x() = bboxmin.x(); P.x() <= bboxmax.x(); P.x()++) {
        for (P.y() = bboxmin.y(); P.y() <= bboxmax.y(); P.y()++) {
            Eigen::Vector3d bc_screen = barycentric(pts, P);
            if (bc_screen.x() < 0 || bc_screen.y() < 0 || bc_screen.z() < 0) continue;
            image.set(P.x(), P.y(), color);
        }
    }
}

void
triangle_l2(Eigen::Vector2i &t0, Eigen::Vector2i &t1, Eigen::Vector2i &t2, TGAImage &image, const TGAColor &color) {
//    lineSweeping_myImplement(t0,t1,t2,image,color);
//    lineSweeping_lessonImplement(t0, t1, t2, image, color);
    boundingBox_myImplement(t0, t1, t2, image, color);
//    boundingBox_lessonImplement(t0, t1, t2, image, color);
}

void triangle_l3(Eigen::Vector3d t0, Eigen::Vector3d t1, Eigen::Vector3d t2, TGAImage &image, double *zBuffer,
                 const std::function<TGAColor(double, double)> &getColor) {
    auto cross = [](Eigen::Vector3d v0, Eigen::Vector3d v1) {
        return v0.x() * v1.y() - v1.x() * v0.y();
    };
    auto isATriangle = [&t0, &t1, &t2, &cross]() {
        Eigen::Vector3d t01 = t1 - t0, t02 = t2 - t0;
        return cross(t01, t02) != 0;
    };
    if (!isATriangle()) {
        return;
    }
    auto barycentric = [&t0, &t1, &t2](int x, int y) -> Eigen::Vector3d { //adopt from barycentric
        Eigen::Vector3d vx = {t1.x() - t0.x(), t2.x() - t0.x(), t0.x() - static_cast<float>(x)};
        Eigen::Vector3d vy = {t1.y() - t0.y(), t2.y() - t0.y(), t0.y() - static_cast<float>(y)};
        Eigen::Vector3d uv1 = {vx.y() * vy.z() - vx.z() * vy.y(), vx.z() * vy.x() - vx.x() * vy.z(),
                               vx.x() * vy.y() - vx.y() * vy.x()};
        if (std::abs(uv1.z()) < 1)
            return {-1, 1, 1};
        double u = uv1.x() / uv1.z(), v = uv1.y() / uv1.z();
        return {1 - u - v, u, v};
    };
    int width = image.get_width();
    int height = image.get_height();
    int fromX = std::max(static_cast<int>(std::min(t0.x(), std::min(t1.x(), t2.x()))), 0),
            toX = std::min(static_cast<int>(std::max(t0.x(), std::max(t1.x(), t2.x()))), width - 1),
            fromY = std::max(static_cast<int>(std::min(t0.y(), std::min(t1.y(), t2.y()))), 0),
            toY = std::min(static_cast<int>(std::max(t0.y(), std::max(t1.y(), t2.y()))), height - 1);


    for (int x = fromX; x <= toX; x++) {
        for (int y = fromY; y <= toY; y++) {
            Eigen::Vector3d bc = barycentric(x, y);
            if (bc.x() < 0 || bc.y() < 0 || bc.z() < 0)
                continue;
            double z = bc.x() * t0.z() + bc.y() * t1.z() + bc.z() * t2.z();
            int index = width * y + x;
            if (z > zBuffer[index]) {
                zBuffer[index] = z;
                image.set(x, y, getColor(bc.y(), bc.z()));
            }
        }
    }
}
