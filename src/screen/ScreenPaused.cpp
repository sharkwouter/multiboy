#include "ScreenPaused.hpp"

#include <filesystem>

#include "../utils.hpp"
#include "../InputType.hpp"

ScreenPaused::ScreenPaused(FontManager * font_manager) {
    this->font_manager = font_manager;
    this->getOptions();
}

ScreenPaused::~ScreenPaused() {
    this->resetTexts();
    if (this->text_paused) {
        SDL_DestroyTexture(this->text_paused);
        this->text_paused = nullptr;
    }
}

void ScreenPaused::handleInput(Input input) {
    switch (input.type) {
        case InputType::UP:
            selected -= 1;
            if (selected < 0) {
                selected = 0;
            }
            this->resetTexts();
            break;
        case InputType::DOWN:
            selected += 1;
            if (selected > (int) this->options.size()) {
                selected = (int) this->options.size();
            }
            this->resetTexts();
            break;
        default:
            break;
    }
}

void ScreenPaused::update() {

}

void ScreenPaused::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    SDL_Rect src_rect;
    SDL_Rect text_rect;

    // Draw create profile options
    if (this->text_paused == nullptr) {
        this->text_paused = this->font_manager->getTexture(renderer, "Paused", FontType::TITLE, {0, 0, 0, 255});
    }
    SDL_QueryTexture(this->text_paused, NULL, NULL, &text_rect.w, &text_rect.h);
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = std::min(text_rect.w, dst_rect->w);
    src_rect.h = text_rect.h;

    text_rect.x = dst_rect->x + dst_rect->w / 2 - text_rect.w / 2;
    text_rect.y = dst_rect->y;
    text_rect.w = std::min(text_rect.w, dst_rect->w);
    SDL_RenderCopy(renderer, this->text_paused, &src_rect, &text_rect);

    // Draw options
    for(int i = 0; i < (int) this->options.size(); i++) {
        if (this->texts[i] == nullptr) {
            if (this->selected == i) {
                this->texts[i] = this->font_manager->getTexture(renderer, this->options[i], FontType::TITLE, {255, 0, 0, 255});
            } else {
                this->texts[i] = this->font_manager->getTexture(renderer, this->options[i], FontType::TITLE, {0, 0, 0, 255});
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

void ScreenPaused::resetTexts() {
    for(int i = 0; i < (int) this->texts.size(); i++) {
        if (this->texts[i] != nullptr) {
            SDL_DestroyTexture(this->texts[i]);
            this->texts[i] = nullptr;
        }
    }
}

void ScreenPaused::getOptions() {
    this->resetTexts();
    this->options.clear();

    for (int option = 0; option < (int) PauseOption::OPTION_COUNT; option++) {
        std::string text = "";
        switch ((PauseOption) option) {
            case PauseOption::CONTINUE:
                text = "Continue";
                break;
            case PauseOption::EXIT:
                text = "Exit";
                break;
            default:
                text = "??????";
                break;
        }
        this->options.push_back(text);
        this->texts.push_back(nullptr);
    }
}

PauseOption ScreenPaused::getSelectedOption() {
    return (PauseOption) selected;
}
