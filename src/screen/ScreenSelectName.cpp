#include "ScreenSelectName.hpp"

#include <filesystem>
#include <memory.h>

#include "../utils.hpp"
#include "../InputType.hpp"
#include "../constants.hpp"

ScreenSelectName::ScreenSelectName(FontManager * font_manager) {
    this->font_manager = font_manager;

    this->name = (char *) malloc(sizeof(char) * (MAX_NAME_LENGTH + 1));
    memset(this->name, '_', MAX_NAME_LENGTH);
    this->name[MAX_NAME_LENGTH] = '\0';
}

ScreenSelectName::~ScreenSelectName() {
    free(this->name);
}

void ScreenSelectName::handleInput(Input input) {
    if(this->nameIsChosen) {
        return;
    }

    switch (input.type) {
        case InputType::B:
            this->name[this->selected] = '_';
            if (this->selected == 0) {
                this->nameIsChosen = true;
            } else {
                this->selected -= 1;
            }
            break;
        case InputType::A:
            if (this->name[this->selected] == '_') {
                if (this->selected > 0) {
                    this->nameIsChosen = true;
                }
                break;
            }
            this->selected += 1;
            if (this->selected > (MAX_NAME_LENGTH - 1)) {
                this->selected = MAX_NAME_LENGTH - 1;
            }
            break;
        case InputType::UP:
            if(this->name[this->selected] == 'A') {
                this->name[this->selected] = '_';
            } else if(this->name[this->selected] == '_') {
                this->name[this->selected] = 'Z';
            } else {
                this->name[this->selected] -= 1;
            }
            break;
        case InputType::DOWN:
            if(this->name[this->selected] == 'Z') {
                this->name[this->selected] = '_';
            } else if(this->name[this->selected] == '_') {
                this->name[this->selected] = 'A';
            } else {
                this->name[this->selected] += 1;
            }
            break;
        default:
            break;
    }
}

void ScreenSelectName::update() {

}

void ScreenSelectName::draw(SDL_Renderer * renderer, SDL_Rect * dst_rect) {
    SDL_Rect selection_rect;
    SDL_Rect text_rect;
    if (this->updated) {
        this->text = this->font_manager->getTexture(renderer, this->name, FontType::TITLE, {0, 0, 0, 255});
    }
    SDL_QueryTexture(this->text, NULL, NULL, &text_rect.w, &text_rect.h);
    text_rect.x = dst_rect->x + (dst_rect->w / 2) - (text_rect.w / 2);
    text_rect.y = dst_rect->y + (dst_rect->h / 2) - (text_rect.h / 2);

    selection_rect.w = text_rect.w / MAX_NAME_LENGTH;
    selection_rect.x = text_rect.x + (selection_rect.w * this->selected);
    selection_rect.y = text_rect.y;
    selection_rect.h = text_rect.h;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &selection_rect);

    SDL_RenderCopy(renderer, this->text, nullptr, &text_rect);
}

std::string ScreenSelectName::getSelectedName() {
    std::string result = "";

    for(int i = 0; i < MAX_NAME_LENGTH; i++) {
        if(this->name[i] == '_') {
            break;
        } else {
            result += this->name[i];
        }
    }

    return result;
}

bool ScreenSelectName::getNameIsChosen()
{
  return this->nameIsChosen;
}
