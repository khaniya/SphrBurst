Camera camera; //camera is made global

class LightSource {
    public:
    Point position;
    double ir, ig, ib;
    LightSource(double ax = 0, double ay = 0, double az = 0, double r = 0, double g = 0, double b = 0)
        : ir(r), ig(g), ib(b) {
            position = Point(ax, ay, az);
        }
	Color calculate_self_intensity(double dist) const {
		static const double MAX_DIST = 1000;
		double res = 1 - dist / MAX_DIST;
		if (res < 0) res = 0;
		return Color(ir, ig, ib) * res;
	}
	//returns normalized color
	Color calculateLightIntensity(Point N, const Point & pixel_pos, const Camera & cam) const {
		Point lsz = position.toVC(cam);
	//        Point lsz ; lsz.x = -90.3924;lsz.y = -42;lsz.z = -430.125;
		double dist = pixel_pos.distanceTo(lsz);
		Color light_intensity = calculate_self_intensity(dist);
		double kd = 0.51; //coefficient of diffusion
		Point L = (lsz - pixel_pos).normalize(); //light normal
		double cosfact = N * L;
		if (cosfact < 0) {
		    N = Point(0, 0, 0) - N; //invert the normal's direction
		}
		cosfact = abs(cosfact);
		Color diffuse = light_intensity * cosfact * kd ;
		//specular begin
		Point V = (Point(0, 0, camera.ze) - pixel_pos).normalize(); //since our camera is placed at (0, 0, ze)
		Point R = N * 2.0 * cosfact  - L;
		double fct = abs(V * R);
		const double ks = 0.31;
		const double ns = 200;
		Color specular(0, 0, 0);
		double K = ks * pow(fct, ns);
		specular = light_intensity * K;
		Color res(specular + diffuse);
		return res;
	}
};


