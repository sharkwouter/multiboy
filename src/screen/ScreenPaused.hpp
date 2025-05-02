#ifndef SCREENPAUSED_HPP
#define SCREENPAUSED_HPP

#include <SDL.h>
#include <vector>
#include <string>
#include <regex>

#include "ScreenBase.hpp"
#include "../Input.hpp"
#include "../FontManager.hpp"
#include "../PauseOption.hpp"

class ScreenPaused : public ScreenBase {
private:
    FontManager * font_manager = nullptr;

    std::vector<std::string> options;
    std::vector<SDL_Texture*> texts;
    SDL_Texture* text_paused = nullptr;
    int selected = 0;

    void resetTexts();
    void getOptions();
public:
    ScreenPaused(FontManager * font_manager);
    ~ScreenPaused();

    PauseOption getSelectedOption();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // SCREENPAUSED_HPP