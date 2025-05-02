#include "ScreenSelectProfile.hpp"

#include <filesystem>

#include "../utils.hpp"
#include "../InputType.hpp"

ScreenSelectProfile::ScreenSelectProfile(FontManager * font_manager) {
    this->font_manager = font_manager;
    this->getProfiles();
}

ScreenSelectProfile::~ScreenSelectProfile() {
    this->resetTexts();
}

void ScreenSelectProfile::handleInput(Input input) {
    switch (input.type)
    {
    case InputType::UP:
        selected -= 1;
        if (selected < 0) {
            selected = 0;
        }
        this->resetTexts();
        break;
    case InputType::DOWN:
        selected += 1;
        if (selected > (int) this->profiles.size()) {
            selected = (int) this->profiles.size();
        }
        this->resetTexts();
        break;
    default:
        break;
    }
}

void ScreenSelectProfile::update() {

}

void ScreenSelectProfile::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    SDL_Rect src_rect;
    SDL_Rect text_rect;

    // Draw create profile options
    if (this->text_new_profile == nullptr) {
        if (this->selected == 0) {
            this->text_new_profile = this->font_manager->getTexture(renderer, this->new_profile_string, FontType::TITLE, {255, 0, 0, 255});
        } else {
            this->text_new_profile = this->font_manager->getTexture(renderer, this->new_profile_string, FontType::TITLE, {255, 255, 255, 255});
        }
    }
    SDL_QueryTexture(this->text_new_profile, NULL, NULL, &text_rect.w, &text_rect.h);
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = std::min(text_rect.w, dst_rect->w);
    src_rect.h = text_rect.h;

    text_rect.x = dst_rect->x;
    text_rect.y = dst_rect->y;
    text_rect.w = std::min(text_rect.w, dst_rect->w);
    SDL_RenderCopy(renderer, this->text_new_profile, &src_rect, &text_rect);

    // Draw profile names
    for(int i = 0; i < (int) this->profiles.size(); i++) {
        if (this->texts[i] == nullptr) {
            if (this->selected - 1 == i) {
                this->texts[i] = this->font_manager->getTexture(renderer, this->profiles[i], FontType::TITLE, {255, 0, 0, 255});
            } else {
                this->texts[i] = this->font_manager->getTexture(renderer, this->profiles[i], FontType::TITLE, {255, 255, 255, 255});
            }
        }
        SDL_QueryTexture(this->texts[i], NULL, NULL, &text_rect.w, &text_rect.h);
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.w = std::min(text_rect.w, dst_rect->w);
        src_rect.h = text_rect.h;

        text_rect.x = dst_rect->x;
        text_rect.y = dst_rect->y + text_rect.h * (i + 1);
        text_rect.w = std::min(text_rect.w, dst_rect->w);
        if ((text_rect.y + text_rect.h) > (dst_rect->y + dst_rect->h)) {
            continue;
        }
        SDL_RenderCopy(renderer, this->texts[i], &src_rect, &text_rect);
    }

}

void ScreenSelectProfile::resetTexts() {
    for(int i = 0; i < (int) this->texts.size(); i++) {
        if (this->texts[i] != nullptr) {
            SDL_DestroyTexture(this->texts[i]);
            this->texts[i] = nullptr;
        }
    }
    if (this->text_new_profile) {
        SDL_DestroyTexture(this->text_new_profile);
        this->text_new_profile = nullptr;
    }
}

void ScreenSelectProfile::getProfiles() {
    this->resetTexts();
    this->profiles.clear();

    std::filesystem::directory_iterator profile_iterator(getProfilePath());
    for (auto &profile : profile_iterator) {
        if (profile.is_directory()) {
            std::string name = profile.path().filename().string();
            this->profiles.push_back(name);
            this->texts.push_back(nullptr);
        }
    }
}

std::string ScreenSelectProfile::getSelectedProfile() {
    if (this->profiles.size() > 0 && selected != 0) {
        return this->profiles[this->selected - 1];
    } else {
        return "";
    }
}
