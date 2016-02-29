#include <SDL/SDL.h>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "common.h"

using namespace std;

int main(int argc, char ** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Surface * screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_Event event;
    bool run = 1;
//    Cuboid cb(0, 0, 0, 100, -50, 200, Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0));
//    Camera Cub_Camera = camera;
    show s(1, 1, 1, 1, 1, 1, 1, 1);
	light_source.position.x = -20;light_source.position.y = -42;light_source.position.z = -437;
//    Cuboid lght(light_source.position.x, light_source.position.y, light_source.position.z, 5, -5, 5, Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1));
//    lght.draw(screen, camera);
	
    while (run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) run = 0;
        }
        //logic
        Uint8 *keys = SDL_GetKeyState(0);
        if (keys[SDLK_a]) camera.phi += 0.1;
        if (keys[SDLK_d]) camera.phi -= 0.1;

        if (keys[SDLK_q]) camera.theta += 0.1;
        if (keys[SDLK_e]) camera.theta -= 0.1;

        const int delta = 2;
        if (keys[SDLK_z]) camera.ze += delta, camera.zv += delta; //zoom in
        if (keys[SDLK_x]) camera.ze -= delta, camera.zv -= delta; //zoom out
//		light_source.position.x = -47;light_source.position.y = -42;light_source.position.z = -437;
//        if (keys[SDLK_k]) light_source.position.z += 1; 
//        if (keys[SDLK_j]) light_source.position.z -= 1;
//        if (keys[SDLK_o]) light_source.position.x += 1;
//        if (keys[SDLK_i]) light_source.position.x -= 1;
//        if (keys[SDLK_m]) light_source.position.y += 1;
//        if (keys[SDLK_n]) light_source.position.y -= 1;
//		cout << light_source.position.x <<", "<<light_source.position.y<<","<<light_source.position.z<<endl;

        if(event.type == SDL_KEYDOWN)
     {
        switch (event.key.keysym.sym){
            case SDLK_1:
                s.s1 = 0;
                break;
            case SDLK_2:
                s.s2 = 0;
                break;
            case SDLK_3:
                s.s3 = 0;
                break;
            case SDLK_4:
                s.s4 = 0;
                break;
            case SDLK_5:
                s.s5 = 0;
                break;
            case SDLK_6:
                s.s6 = 0;
                break;
            case SDLK_7:
                s.s7 = 0;
                break;
            case SDLK_8:
                s.s8 = 0;
                break;

            }
     }

        //rendering
        SDL_FillRect(screen, &screen->clip_rect, 0	);
        clearZBuffer();
        draw8Sphere(screen, Point(0, 0, 0), 100, camera, s);
        //cb.draw(screen, camera);
//        Cuboid lght(light_source.position.x, light_source.position.y, light_source.position.z, 5, -5, 5, Color(0, 1, 0), Color(1, 0, 1), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0), Color(1, 0, 0));
//        lght.draw(screen, camera);
        //lght.draw(screen, Cub_Camera);

        SDL_Flip(screen);
//        SDL_Delay(10);
    }
    SDL_Quit();
}
