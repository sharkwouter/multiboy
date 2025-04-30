#include "FontManager.hpp"

#include <stdexcept>

#include "constants.hpp"

FontManager::FontManager()
{
  if (TTF_Init() == -1)
  {
    throw std::runtime_error(TTF_GetError());
  }

  if (!this->loadFonts(".")) {
    if (!this->loadFonts("../share/multiboy")) {
        throw std::runtime_error(TTF_GetError());
    }
  }
}

FontManager::~FontManager() {
    TTF_CloseFont(font_title);
    TTF_CloseFont(font);
    TTF_CloseFont(font_small);
    TTF_Quit();
}

SDL_Texture * FontManager::getTexture(SDL_Renderer *renderer, std::string text, FontType font_type, SDL_Color color={0,0,0,255}) {
    TTF_Font * current_font = nullptr;
    switch (font_type)
    {
    case FontType::NORMAL:
        current_font = this->font;
        break;
    case FontType::TITLE:
        current_font = this->font_title;
        break;
    case FontType::SMALL:
        current_font = this->font_small;
        break;
    }
    
    SDL_Surface * surface = TTF_RenderUTF8_Blended(current_font, text.c_str(), color);
    if (surface == NULL) {
        SDL_Log("Couldn't create surface for text %s: %s", text.c_str(), TTF_GetError());
        return NULL;
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

bool FontManager::loadFonts(std::string location) {
    std::string font_path = location + std::string("/assets/PublicPixel.ttf");

    this->font_small = TTF_OpenFont(font_path.c_str(), FONT_SIZE / 2);
    this->font = TTF_OpenFont(font_path.c_str(), FONT_SIZE);
    this->font_title = TTF_OpenFont(font_path.c_str(), FONT_SIZE * 2);

    if (!this->font_small || !this->font || !this->font_title) {
      return false;
    }

    return true;
}
