#ifndef SCREENSELECTNAME_HPP
#define SCREENSELECTNAME_HPP

#include <SDL.h>
#include <string>

#include "ScreenBase.hpp"
#include "../FontManager.hpp"

class ScreenSelectName : public ScreenBase {
private:
    FontManager * font_manager = nullptr;

    bool updated = true;
    char * name;
    SDL_Texture* text = nullptr;
    int selected = 0;
    int nameIsChosen = false;
public:
    ScreenSelectName(FontManager * font_manager);
    ~ScreenSelectName();

    std::string getSelectedName();
    bool getNameIsChosen();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // SCREENSELECTNAME_HPP