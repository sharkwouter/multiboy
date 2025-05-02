#include "ScreenSelectRom.hpp"

#include <filesystem>

#include "../utils.hpp"
#include "../InputType.hpp"

ScreenSelectRom::ScreenSelectRom(FontManager * font_manager) {
    this->font_manager = font_manager;
    this->getRoms();
}

ScreenSelectRom::~ScreenSelectRom() {
    // TODO: Clean up arrays here
}

void ScreenSelectRom::handleInput(Input input) {
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
        if (selected >= (int) this->roms.size()) {
            selected = (int) this->roms.size() -1;
        }
        this->resetTexts();
        break;
    default:
        break;
    }
}

void ScreenSelectRom::update() {

}

void ScreenSelectRom::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    SDL_Rect src_rect;
    SDL_Rect text_rect;
    for(int i = this->selected; i < (int) this->roms.size(); i++) {
        if (this->texts[i] == nullptr) {
            if (this->selected == i) {
                this->texts[i] = this->font_manager->getTexture(renderer, this->roms[i], FontType::TITLE, {255, 0, 0, 255});
            } else {
                this->texts[i] = this->font_manager->getTexture(renderer, this->roms[i], FontType::TITLE, {0, 0, 0, 255});
            }
        }
        SDL_QueryTexture(this->texts[i], NULL, NULL, &text_rect.w, &text_rect.h);
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.w = std::min(text_rect.w, dst_rect->w);
        src_rect.h = text_rect.h;

        text_rect.x = dst_rect->x;
        text_rect.y = dst_rect->y + text_rect.h*(i-selected);
        text_rect.w = std::min(text_rect.w, dst_rect->w);
        if ((text_rect.y + text_rect.h) > (dst_rect->y + dst_rect->h)) {
            continue;
        }
        SDL_RenderCopy(renderer, this->texts[i], &src_rect, &text_rect);
    }

    // Print rom location if no roms are found
    if (this->roms.size() == 0) {
        if (no_roms_found_texture == nullptr) {
            std::string no_roms_text = "Please put roms in " + getRomPath();
            no_roms_found_texture = this->font_manager->getTexture(renderer, no_roms_text, FontType::TITLE, {255, 0, 0, 255});
        }
        SDL_QueryTexture(no_roms_found_texture, NULL, NULL, &text_rect.w, &text_rect.h);
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.w = std::min(text_rect.w, dst_rect->w);
        src_rect.h = text_rect.h;

        text_rect.x = dst_rect->x;
        text_rect.y = dst_rect->y;
        text_rect.w = std::min(text_rect.w, dst_rect->w);
        SDL_RenderCopy(renderer, no_roms_found_texture, &src_rect, &text_rect);
    }
}

void ScreenSelectRom::resetTexts() {
    for(int i = 0; i < (int) this->texts.size(); i++) {
        if (this->texts[i] != nullptr) {
            SDL_DestroyTexture(this->texts[i]);
            this->texts[i] = nullptr;
        }
    }
}

void ScreenSelectRom::getRoms() {
    this->resetTexts();
    this->roms.clear();

    std::filesystem::directory_iterator rom_iterator(getRomPath());
    for (auto &rom : rom_iterator) {
        if (std::regex_match(rom.path().string(), rom_regex)) {
            std::string name = rom.path().filename().string();
            this->roms.push_back(name);
            this->texts.push_back(nullptr);
        }
    }
}

std::string ScreenSelectRom::getSelectedRom() {
    if (this->roms.size() > 0) {
        return getRomPath() + "/" + this->roms[this->selected];
    } else {
        return "";
    }
}
