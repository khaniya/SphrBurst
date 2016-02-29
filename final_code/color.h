#include <iostream>
using namespace std;
#include <stdint.h>

struct Color {
    double r, g, b;
    Color(double ar = 0, double ag = 0, double ab = 0) : r(ar), g(ag), b(ab) { }
    uint32_t getColor() const ;
    Color operator*(const double v) const {
        return Color(r * v, g * v, b * v);
    }
	static void boundVariable(double & val) {
		if (val > 1) val = 1;
		if (val < 0) val = 0; 
	}
    Color bound() const ;
    Color operator+(const Color & op2) const {
        return Color(r + op2.r, g + op2.g, b + op2.b);
    }
}; 

