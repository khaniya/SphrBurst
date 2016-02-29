#include "color.h"

uint32_t Color::getColor() const {
    if (r < 0 || r > 1) cout << "red error\n";
    if (g < 0 || g > 1) cout << "green error\n";
    if (b < 0 || b > 1) cout << "blue error\n";
    int rv = r * 255;
    int gv = g * 255;
    int bv = b * 255;
    if (rv < 0) rv = 0;
    if (gv < 0) gv = 0;
    if (bv < 0) bv = 0; 
    rv &= 255;
    gv &= 255;
    bv &= 255;
    return (rv << 16) | (gv << 8) | bv;
}
Color Color::bound() const {
    Color res = *this;
    boundVariable(res.r);
    boundVariable(res.g);
    boundVariable(res.b);
    return res;
}


