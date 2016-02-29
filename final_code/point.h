#include "camera.h"
#include <cmath>
#include "color.h"
#include "matrix.h"

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

class Point {
    public:
    double x, y, z;
    Color color;
    Point(double cx = 0, double cy = 0, double cz = 0, const Color & pcol = Color(.6, .6, .6)):
        x(cx), y(cy), z(cz), color(pcol) {
        }
    //vector math
    Point operator-(const Point & b) const {return Point(x - b.x, y - b.y, z - b.z, color);}
    Point operator*(double factor) const {return Point(x * factor, y * factor, z * factor, color);}
    double operator*(const Point & b) const {return x * b.x + y * b.y + z * b.z;}
    Point operator+(const Point & b) const {return Point(x + b.x, y + b.y, z + b.z, color);}
    //transformations
	Point rotateY(double t) const;
    Point rotateZ(double t) const;
    Point rotateX(double t) const ;
	Point translate(const Point& tMat) const;	
	Point scale(const Point& sMat) const;
    //crude pipelining steps
    Point toVC(const Camera & cam) const ;
    Point project(const Camera & cam) const ;
    Point to2dview() const ;
    double distanceTo(const Point & b) const ;
    double magnitude() const ;
    Point normalize() const ;
    double crossZ(const Point & b) const ;//returns only the z component of the cross product
    Point cross(const Point & b) const ;
}; 

