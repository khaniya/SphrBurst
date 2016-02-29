#include "point.h"

//transformations
Point Point::rotateY(double theta) const {
	Matrix T(4,4); //Transformation matrix
	Matrix P(4,1); //Point matrix
	//Transformation matrix
	T(0,0) = cos(theta);    T(0,1) = 0;         T(0,2) = sin(theta);    T(0,3) = 0;
	T(1,0) = 0;             T(1,1) = 1;         T(1,2) = 0;             T(1,3) = 0;
	T(2,0) = -sin(theta);   T(2,1) = 0;         T(2,2) = cos(theta);    T(2,3) = 0;
	T(3,0) = 0;             T(3,1) = 0;         T(3,2) = 0;             T(3,3) = 1;
	//Point in matrix form
	P(0,0) = x; P(0,1) = y; P(0,2) = z; P(0,3) = 1;

	P = T*P;
	double nx = P(0,0);
	double ny = P(0,1);
	double nz = P(0,2);
	return Point(nx, ny, nz, color);
}

Point Point::rotateZ(double theta) const {
	Matrix T(4,4); //Transformation matrix
	Matrix P(4,1); //Point matrix
	//Transformation matrix
	T(0,0) = cos(theta);    T(0,1) = -sin(theta);       T(0,2) = 0;    T(0,3) = 0;
	T(1,0) = sin(theta);    T(1,1) = cos(theta);        T(1,2) = 0;    T(1,3) = 0;
	T(2,0) = 0;             T(2,1) = 0;                 T(2,2) = 1;    T(2,3) = 0;
	T(3,0) = 0;             T(3,1) = 0;                 T(3,2) = 0;    T(3,3) = 1;
	//Point in matrix form
	P(0,0) = x; P(0,1) = y; P(0,2) = z; P(0,3) = 1;

	P = T*P;
	double nx = P(0,0);
	double ny = P(0,1);
	double nz = P(0,2);
	return Point(nx, ny, nz, color);
}


Point Point::rotateX(double theta) const {
	Matrix T(4,4); //Transformation matrix
	Matrix P(4,1); //Point matrix
	//Transformation matrix
	T(0,0) = 1;     T(0,1) = 0;         T(0,2) = 0;             T(0,3) = 0;
	T(1,0) = 0;     T(1,1) = cos(theta);T(1,2) = -sin(theta);   T(1,3) = 0;
	T(2,0) = 0;     T(2,1) = sin(theta);T(2,2) = cos(theta);    T(2,3) = 0;
	T(3,0) = 0;     T(3,1) = 0;         T(3,2) = 0;             T(3,3) = 1;
	//Point in matrix form
	P(0,0) = x; P(0,1) = y; P(0,2) = z; P(0,3) = 1;

	P = T*P;
	double nx = P(0,0);
	double ny = P(0,1);
	double nz = P(0,2);
	return Point(nx, ny, nz, color);
}

Point Point::translate(const Point& tMat) const {
	Matrix T(4,4); //Transformation matrix
	Matrix P(4,1); //Point matrix
	//Transformation matrix
	T(0,0) = 1;    T(0,1) = 0;      T(0,2) = 0;    T(0,3) = tMat.x;
	T(1,0) = 0;    T(1,1) = 1;      T(1,2) = 0;    T(1,3) = tMat.y;
	T(2,0) = 0;    T(2,1) = 0;      T(2,2) = 1;    T(2,3) = tMat.z;
	T(3,0) = 0;    T(3,1) = 0;      T(3,2) = 0;    T(3,3) = 1;
	//Point in matrix form
	P(0,0) = x; P(0,1) = y; P(0,2) = z; P(0,3) = 1;

	P = T*P;
	double nx = P(0,0);
	double ny = P(0,1);
	double nz = P(0,2);
	return Point(nx, ny, nz, color);

}

Point Point::scale(const Point& sMat) const {
	Matrix T(4,4); //Transformation matrix
	Matrix P(4,1); //Point matrix
	//Transformation matrix
	T(0,0) = sMat.x;    T(0,1) = 0;      T(0,2) = 0;    	T(0,3) = 0;
	T(1,0) = 0;    	T(1,1) = sMat.y; T(1,2) = 0;    	T(1,3) = 0;
	T(2,0) = 0;    	T(2,1) = 0;      T(2,2) = sMat.z;    	T(2,3) = 0;
	T(3,0) = 0;    	T(3,1) = 0;      T(3,2) = 0;    	T(3,3) = 1;
	//Point in matrix form
	P(0,0) = x; P(0,1) = y; P(0,2) = z; P(0,3) = 1;

	P = T*P;
	double nx = P(0,0);
	double ny = P(0,1);
	double nz = P(0,2);
	return Point(nx, ny, nz, color);

}
//Point Point::rotateY(double t) const {
//	double nz = z * cos(t) - x * sin(t);
//	double nx = z * sin(t) + x * cos(t);
//	return Point(nx, y, nz, color);
//}
//Point Point::rotateZ(double t) const {
//    double nx = x * cos(t) - y * sin(t);
//    double ny = x * sin(t) + y * cos(t);
//    return Point(nx, ny, z, color);
//}
//Point Point::rotateX(double t) const {
//    double ny = y * cos(t) - z * sin(t);
//    double nz = y * sin(t) + z * cos(t);
//    return Point(x, ny, nz, color);
//}

//crude pipelining steps
Point Point::toVC(const Camera & cam) const {
    return rotateY(-cam.phi).rotateX(-cam.theta);
}
Point Point::project(const Camera & cam) const {
    double f = (cam.ze - cam.zv) / (cam.ze - z);
    double xp = x * f, yp = y * f;
    return Point(xp, yp, cam.zv, color);
}
Point Point::to2dview() const {
    return Point(x + SCREEN_WIDTH / 2, y + SCREEN_HEIGHT / 2, z, color);
}
double Point::distanceTo(const Point & b) const {
    return (*this - b).magnitude();
}
double Point::magnitude() const {
    return sqrt(x * x + y * y + z * z);
}
Point Point::normalize() const {
    double v = magnitude(); //assert v != 0
    return Point(x / v, y / v, z / v, color);
}
double Point::crossZ(const Point & b) const {//returns only the z component of the cross product
    return x * b.y - y * b.x;
}
Point Point::cross(const Point & b) const {
    double rx = y * b.z - z * b.y;
    double ry = -(x * b.z - z * b.x);
    double rz = x * b.y - y * b.x;
    return Point(rx, ry, rz, color);
}

