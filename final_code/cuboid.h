struct Cuboid {
    Point a, b, c, d, e, f, g, h;
    Cuboid(double ox, double oy, double oz, double length, double breadth, double height, const Color & cola, const Color & colb, const Color & colc, const Color & cold, const Color & cole, const Color & colf, const Color & colg, const Color & colh) {
        a = Point(ox, oy, oz, cola);
        b = Point(ox + length, oy, oz, colb);
        c = Point(ox + length, oy + breadth, oz, colc);
        d = Point(ox, oy + breadth, oz, cold);
        e = Point(ox, oy, oz + height, cole);
        f = Point(ox + length, oy, oz + height, colf);
        g = Point(ox + length, oy + breadth, oz + height, colg);
        h = Point(ox, oy + breadth, oz + height, colh);
    }
    void draw(SDL_Surface * screen, const Camera & cam) const {
        triangleFill(screen, a, b, c, cam);
        triangleFill(screen, a, d, c, cam);
        triangleFill(screen, e, f, g, cam);
        triangleFill(screen, e, h, g, cam);

        triangleFill(screen, a, d, e, cam);
        triangleFill(screen, h, d, e, cam);

        triangleFill(screen, b, f, g, cam);
        triangleFill(screen, c, b, g, cam);

        triangleFill(screen, a, e, b, cam);
        triangleFill(screen, e, b, f, cam);
    }
};

