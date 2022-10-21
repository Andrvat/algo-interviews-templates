#include <iostream>
#include <vector>
#include <complex>

using namespace std;

/*

Уравнение прямой через две точки:

(x-x1)/(x2-x1) = (y-y1)/(y2-y1)

отсюда:

x*(y2-y1) + x1*(y1-y2) + y*(x2-x1) + y1*(x2-x1) = 0

таким образом для уравнения прямой Ax + By + C = 0 имеем

A = y2 - y1

B = x1 - x2

C = x1 * (y1 - y2) + y1 * (x2 - x1)

Расстояние от точки M (x, y) до прямой считаем по формуле:

d = Math.Abs(A * Mx + B * My + C) / Math.Sqrt(A^2 + B^2)

Вот это расстояние уже можно сравнивать с некой величиной погрешности

*/

struct Point {
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {
    }
};


bool isOnOneLine(const vector<Point> &points) {
    if (points.size() < 2) {
        return false;
    }
    Point base1 = points[0];
    int32_t startPos = 1;
    Point base2 = points[startPos];
    while (base1.x == base2.x && base1.y == base2.y && startPos < points.size() - 1) {
        startPos++;
        base2 = points[startPos];
    }
    if (startPos == points.size() - 1) {
        return true;
    }
    auto a = static_cast<double>(base2.y - base1.y);
    auto b = static_cast<double>(base1.x - base2.x);
    auto c = -a * base1.x - b * base1.y;
    auto evk = std::sqrt(a * a + b * b);
    for (int i = startPos + 1; i < points.size(); ++i) {
        auto &point = points[i];
        auto dest = std::abs(a * point.x + b * point.y + c) / evk;
        if (dest > 1e-323) {
            return false;
        }
    }
    return true;
}

int main() {
    int64_t n;
    cin >> n;
    vector<Point> points;
    for (int i = 0; i < n; i++) {
        int64_t x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
    }
    if (isOnOneLine(points)) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}