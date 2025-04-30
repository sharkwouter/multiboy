#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#include "FontType.hpp"

class FontManager {
private:
    TTF_Font * font_title = nullptr;
    TTF_Font * font = nullptr;
    TTF_Font * font_small = nullptr;
public:
    FontManager();
    ~FontManager();

    SDL_Texture * getTexture(SDL_Renderer *renderer, std::string text, FontType font_type, SDL_Color color);
};

#endif // FONTMANAGER_HPP