#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "An Implementation of Code.org Painter";
void logSDLError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
      // SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&//SDL_PollEvent(&e)
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, "anh2.png");
    SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = 800;
        src.h = 369;
    SDL_Rect dst;
        dst.x = 0;
        dst.y = 0;
        dst.w = 800;
        dst.h = 600;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &src, &dst);
    SDL_RenderPresent(renderer);

    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}

