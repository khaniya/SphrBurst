#include "point.h"
#include "ltsrc.h"

#define sign(x) ((x) >= 0 ? 1 : -1)

LightSource light_source(20, -10, -50, 1, 1, 1);

void setpixel(SDL_Surface * screen, int x, int y, Uint32 color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return ;
    Uint32 * pixels = (Uint32*)screen->pixels;
    pixels[y * SCREEN_WIDTH + x] = color;
}

double z_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

double interpolate(double a1, double b1, double a2, double b2, double a) {
    return b1 + (b2 - b1) * (a - a1) / (a2 - a1);
}

Color colorInterpolate(double a, const Color & cola, double b, const Color & colb, double pa) {
    double r = interpolate(a, cola.r, b, colb.r, pa);
    double g = interpolate(a, cola.g, b, colb.g, pa);
    double blue = interpolate(a, cola.b, b, colb.b, pa);
    return Color(r, g, blue);
}

bool pointInsideTriangle(const Point & p, const Point & a, const Point & b, const Point & c) {
    double x = (p - a).crossZ(b - a);
    double y = (p - b).crossZ(c - b);
    double z = (p - c).crossZ(a - c);
    return (x >= 0 && y >= 0 && z >= 0) || (x < 0 && y < 0 && z < 0);
}

//assumes a, b, c are sorted according to y coordinates, i.e. a.y <= b.y <= c.y
//returns the scanline range according to the yposition
void getEndPoints
(const Point & a, const Point & b, const Point & c, double ypos, double & xstart, double & xend, Color & cola, Color & colb) {
    xstart = xend = 0;//dummy
    if (a.y == c.y) return ; //straight line with slope 0, do nothing
    if (a.y == b.y || ypos > b.y) {
        //intersection of y = ypos with AC and BC   
        double intac = interpolate(a.y, a.x, c.y, c.x, ypos);
        double intbc = interpolate(b.y, b.x, c.y, c.x, ypos);
        cola = colorInterpolate(a.y, a.color, c.y, c.color, ypos);
        colb = colorInterpolate(b.y, b.color, c.y, c.color, ypos);
        xstart = intac;
        xend = intbc;
    } else {
        double intab = interpolate(a.y, a.x, b.y, b.x, ypos);
        double intac = interpolate(a.y, a.x, c.y, c.x, ypos);
        cola = colorInterpolate(a.y, a.color, b.y, b.color, ypos);
        colb = colorInterpolate(a.y, a.color, c.y, c.color, ypos);
        xstart = intab;
        xend = intac;
    }
    if (xstart > xend) {
        swap(xstart, xend);
        swap(cola, colb);
    }
}

//z buffering implemented here
void triangleFill(SDL_Surface * screen, Point a, Point b, Point c, const Camera & cam) {
    Point vca = a.toVC(cam), vcb = b.toVC(cam), vcc = c.toVC(cam);
    Point normal = (vcb - vca).cross(vcc - vca); //obtain the triangle normal, i.e. a, b, c components of the plane
    Point normalized = normal.normalize();
    double d = -(normal.x * vca.x + normal.y * vca.y + normal.z * vca.z); // ax + by + cz + d = 0, d = -(ax + by + cz), put point vca
    Point pa = vca.project(cam).to2dview(), pb = vcb.project(cam).to2dview(), pc = vcc.project(cam).to2dview();
    if (pa.y > pb.y) swap(pa, pb);
    if (pb.y > pc.y) swap(pb, pc);
    if (pa.y > pb.y) swap(pa, pb);
    if (pa.y == pc.y) return ;
    double miny = min(pa.y, min(pb.y, pc.y));
    double maxy = max(pa.y, max(pb.y, pc.y));
    double minx = min(pa.x, min(pb.x, pc.x));
    double maxx = max(pa.x, max(pb.x, pc.x));
    for (int i = miny; i <= maxy; ++i) {
        if (i < 0) continue;
        if (i >= SCREEN_HEIGHT) break;
        bool inside = 0;
        double xs, xe;
        Color cols, cole;
        bool calculated = 0;
        bool samesign = 0;
        for (int j = max(minx, 0.0); j <= maxx; ++j) {
            if (j >= SCREEN_WIDTH) break;
            if (pointInsideTriangle(Point(j, i, 0), pa, pb, pc)) {
                if (!inside) {
                    getEndPoints(pa, pb, pc, i, xs, xe, cols, cole);
                    inside = 1;
                }
                double D = cam.ze - cam.zv;
                double xp = j - SCREEN_WIDTH / 2, yp = i - SCREEN_HEIGHT / 2;
                double F = normal.x * xp + normal.y * yp - normal.z * D;
                double point_z = ((normal.x * xp + normal.y * yp) * cam.ze + d * D) / F;
                Color current = colorInterpolate(xs, cols, xe, cole, j);
                if (point_z < z_buffer[i][j]) {
                    z_buffer[i][j] = point_z;
                    Color dcol(0.2, 0.2, 0.2); // ambient ligp ht
                    double f = (cam.ze - point_z) / D;
                    Point vwcPos(xp * f, yp * f, point_z); //use inverse projection to calculate the viewing coordinates of the pixel
                    //only if the light is on the same side as that of camera wrt to this pixels' viewing coordinate point
                    //we add the contribution to dcol
                   	Camera lcam;
                    if (!calculated) {
                        calculated = 1;
                        Point lght = light_source.position.toVC(lcam);
//                        Point lght; lght.x = -90.3924;lght.y = -42;lght.z = -430.125; 

                        double diff1 = (normal * lght) + d;
                        double diff2 = normal.z * lcam.ze + d;
                        samesign = sign(diff1) == sign(diff2);
                    }
                    if (samesign) { //necessary condition for two points to lie on the same side of a plane
                        dcol = dcol + light_source.calculateLightIntensity(normalized, vwcPos, lcam);
                    }
                    if (dcol.r > 1) dcol.r = 1;
                    if (dcol.g > 1) dcol.g = 1;
                    if (dcol.b > 1) dcol.b = 1;
                    current.r *= dcol.r;
                    current.g *= dcol.g;
                    current.b *= dcol.b;
                    setpixel(screen, j, i, current.getColor());
                }
            } else if (inside) break;
        }
    }
}

void drawSphere(SDL_Surface * screen, const Point & center, double radius, const Camera & cam, int fps) {
  int phi_steps = fps, theta_steps = fps;
  double ang_phi = 2 * M_PI / phi_steps, ang_theta = M_PI / theta_steps;
  Point r(0, 0, radius);
  for (int i = 0; i < phi_steps; ++i) {
    double theta = -M_PI / 2.0;
    for (int j = 0; j < theta_steps; ++j) {
      Point a = r.rotateX(theta).rotateY(i * ang_phi);
      Point b = r.rotateX(theta);
      if (i != phi_steps - 1) b = b.rotateY(i * ang_phi + ang_phi);
      Point c = r.rotateX(theta + ang_theta).rotateY(i * ang_phi);
      Point d = r.rotateX(theta + ang_theta).rotateY(i * ang_phi + ang_phi);
      triangleFill(screen, a + center, b + center, c + center, cam);
      triangleFill(screen, d + center, b + center, c + center, cam);
      theta += ang_theta;
    }
  }
}

void draw8SmallSphere(SDL_Surface * screen, const Point & center, double radius, const Camera & camera, int fps) {
    drawSphere(screen, Point(center.x-radius, center.y-radius, center.z-radius), radius, camera, fps);
    drawSphere(screen, Point(center.x+radius, center.y-radius, center.z-radius), radius, camera, fps);
    drawSphere(screen, Point(center.x-radius, center.y+radius, center.z-radius), radius, camera, fps);
    drawSphere(screen, Point(center.x+radius, center.y+radius, center.z-radius), radius, camera, fps);
    drawSphere(screen, Point(center.x-radius, center.y-radius, center.z+radius), radius, camera, fps);
    drawSphere(screen, Point(center.x+radius, center.y-radius, center.z+radius), radius, camera, fps);
    drawSphere(screen, Point(center.x-radius, center.y+radius, center.z+radius), radius, camera, fps);
    drawSphere(screen, Point(center.x+radius, center.y+radius, center.z+radius), radius, camera, fps);
}

void draw8Sphere(SDL_Surface * screen, const Point & center, double radius, const Camera & camera, show s) {
    int fps = 50;
    if(s.s1){ drawSphere(screen, Point(center.x-radius, center.y-radius, center.z-radius), radius, camera, fps);}
    else {
        double a = center.x - radius, b= center.y - radius, c= center.z - radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);

    }

    if(s.s2){drawSphere(screen, Point(center.x+radius, center.y-radius, center.z-radius), radius, camera, fps);}
    else {
        double a = center.x + radius, b= center.y - radius, c= center.z - radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);

    }
    if(s.s3){drawSphere(screen, Point(center.x-radius, center.y+radius, center.z-radius), radius, camera, fps);}
    else {
        double a = center.x - radius, b= center.y + radius, c= center.z - radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);
    }
    if(s.s4){drawSphere(screen, Point(center.x+radius, center.y+radius, center.z-radius), radius, camera, fps);}
    else {
        double a = center.x + radius, b= center.y + radius, c= center.z - radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);
    }
    if(s.s5){drawSphere(screen, Point(center.x-radius, center.y-radius, center.z+radius), radius, camera, fps);}
    else {
        double a = center.x - radius, b= center.y - radius, c= center.z + radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);
    }
    if(s.s6){drawSphere(screen, Point(center.x+radius, center.y-radius, center.z+radius), radius, camera, fps);}
    else {
        double a = center.x + radius, b= center.y - radius, c= center.z + radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);
    }
    if(s.s7){drawSphere(screen, Point(center.x-radius, center.y+radius, center.z+radius), radius, camera, fps);}
    else {
        double a = center.x - radius, b= center.y + radius, c= center.z + radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);
    }
    if(s.s8){drawSphere(screen, Point(center.x+radius, center.y+radius, center.z+radius), radius, camera, fps);}
    else {
        double a = center.x + radius, b= center.y + radius, c= center.z + radius;
        double rad = radius/2;
        draw8SmallSphere(screen, Point(a, b, c), rad, camera, fps/2);
    }
}

void clearZBuffer() {
    for (int i =  0; i < SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) z_buffer[i][j] = 999999999.0; //infinity
    }
}



