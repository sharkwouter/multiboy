#ifndef SCREENSELECTPROFILE_HPP
#define SCREENSELECTPROFILE_HPP

#include <SDL.h>
#include <vector>
#include <string>
#include <regex>

#include "ScreenBase.hpp"
#include "../Input.hpp"
#include "../FontManager.hpp"

class ScreenSelectProfile : public ScreenBase {
private:
    FontManager * font_manager = nullptr;

    std::vector<std::string> profiles;
    std::vector<SDL_Texture*> texts;
    SDL_Texture* text_new_profile;
    int selected = 0;

    std::string new_profile_string = "Create new profile";

    void resetTexts();
    void getProfiles();
public:
    ScreenSelectProfile(FontManager * font_manager);
    ~ScreenSelectProfile();

    std::string getSelectedProfile();

    void handleInput(Input input);
    void update();
    void draw(SDL_Renderer * renderer, SDL_Rect * dst_rect);
};

#endif // SCREENSELECTPROFILE_HPP