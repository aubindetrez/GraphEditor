#include <iostream>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT (WINDOW_WIDTH)

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, const char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderUTF8_Solid(font, text, textColor);

    // Problem: Cannot configure/disable word wrap
    //surface = TTF_RenderUTF8_Solid_Wrapped(font, text, textColor, 120);

    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Rect rect1, rect2;
    SDL_Renderer *renderer;
    SDL_Texture *texture1, *texture2;
    SDL_Window *window;
    const char *font_path;
    int quit;

    if (argc == 1) {
        font_path = "/home/phileas/.fonts/JetBrainsMono/ttf/JetBrainsMono-SemiBold.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    /* Inint TTF. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, 20);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
    TTF_SetFontHinting(font, TTF_HINTING_MONO);

    int font_heigth = TTF_FontHeight(font);
    std::cout << "Font height: " << font_heigth << std::endl;

    get_text_and_rect(renderer, 0, 0, "Main Area ⇨", font, &texture1, &rect1);
    get_text_and_rect(renderer, 0, rect1.y + rect1.h, "Bottom Text", font, &texture2, &rect2);

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = 1;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Draw windows
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        int w, h;
        SDL_GetWindowSizeInPixels(window, &w, &h);
        SDL_RenderDrawLine(renderer, 0, h-2*font_heigth, w, h-2*font_heigth);

        // Draw Images...

        // Draw figures

        // Draw Texts
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        rect2.x = font_heigth/2;
        rect2.y = h-1.5*font_heigth;
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
