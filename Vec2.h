#include <cmath>

template<typename T>
class Vec2 {
public:
    T x, y;

    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}

    // Copy assignment
    Vec2& operator=(const Vec2& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    // Compound assignment: +=, -=, *=, /=
    Vec2& operator+=(const Vec2& v) {
        x += v.x; y += v.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& v) {
        x -= v.x; y -= v.y;
        return *this;
    }

    Vec2& operator*=(T scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }

    Vec2& operator/=(T scalar) {
        x /= scalar; y /= scalar;
        return *this;
    }

    // Arithmetic: Vec2 + Vec2
    Vec2 operator+(const Vec2& v) const {
        return Vec2(x + v.x, y + v.y);
    }

    Vec2 operator-(const Vec2& v) const {
        return Vec2(x - v.x, y - v.y);
    }

    // Arithmetic: Vec2 * scalar, Vec2 / scalar
    Vec2 operator*(T scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator/(T scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    // Optional: Vec2 + scalar, Vec2 - scalar
    Vec2 operator+(T scalar) const {
        return Vec2(x + scalar, y + scalar);
    }

    Vec2 operator-(T scalar) const {
        return Vec2(x - scalar, y - scalar);
    }
    T length() const {
            return std::sqrt(x * x + y * y);
        }

    Vec2<T> rotated(double angle) const {
        double cs = std::cos(angle);
        double sn = std::sin(angle);
        return Vec2<T>(x * cs - y * sn, x * sn + y * cs);
    }

};
