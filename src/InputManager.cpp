#include "InputManager.hpp"

InputManager::InputManager(std::vector<PlayerManager *> * player_managers) {
    this->player_managers = player_managers;
}

InputManager::~InputManager() {

}

void InputManager::getInputs(std::vector<Input> * inputs) {
    inputs->clear();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                this->quit_triggered = true;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                this->connectGamepad(event.cdevice.which);
                SDL_Log("Detecting device added");
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                this->disconnectGamepad();
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                {
                    Input input = this->getGamepadInput(event.cbutton, false);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
            case SDL_CONTROLLERBUTTONUP:
                {
                    Input input = this->getGamepadInput(event.cbutton, true);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
            case SDL_KEYDOWN:
                {
                    Input input = this->getKeyboardInput(event.key.keysym.sym, false);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
            case SDL_KEYUP:
                {
                    Input input = this->getKeyboardInput(event.key.keysym.sym, true);
                    if (input.type != InputType::NONE) {
                        inputs->push_back(input);
                    }
                }
                break;
        }
    }
}

Input InputManager::getGamepadInput(SDL_ControllerButtonEvent event, bool released) {
    Input input;
    input.gamepad_id = event.which;

    switch (event.button)
    {
        case SDL_CONTROLLER_BUTTON_BACK:
            input.type = InputType::SELECT;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            input.type = InputType::START;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            input.type = InputType::A;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            input.type = InputType::B;
            break;
        case SDL_CONTROLLER_BUTTON_Y:
            input.type = InputType::PAUSE;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            input.type = InputType::UP;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            input.type = InputType::DOWN;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            input.type = InputType::LEFT;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            input.type = InputType::RIGHT;
            break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            input.type = InputType::L;
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            input.type = InputType::R;
            break;
        default:
            input.type = InputType::NONE;
    }

    if (released && input.type != InputType::NONE) {
        input.type = (InputType) ((int) input.type + 1);
    }

    return input;
}

Input InputManager::getKeyboardInput(SDL_Keycode button, bool released) {
    Input input;
    input.gamepad_id = KEYBOARD_ID;

    switch (button)
    {
        case SDLK_ESCAPE:
            input.type = InputType::PAUSE;
            break;
        case SDLK_BACKSPACE:
            input.type = InputType::SELECT;
            break;
        case SDLK_RETURN:
            input.type = InputType::START;
            break;
        case SDLK_z:
            input.type = InputType::A;
            break;
        case SDLK_x:
            input.type = InputType::B;
            break;
        case SDLK_UP:
            input.type = InputType::UP;
            break;
        case SDLK_DOWN:
            input.type = InputType::DOWN;
            break;
        case SDLK_LEFT:
            input.type = InputType::LEFT;
            break;
        case SDLK_RIGHT:
            input.type = InputType::RIGHT;
            break;
        case SDLK_a:
            input.type = InputType::L;
            break;
        case SDLK_s:
            input.type = InputType::R;
            break;
        default:
            input.type = InputType::NONE;
    }

    if (released && input.type != InputType::NONE) {
        input.type = (InputType) ((int) input.type + 1);
    }

    return input;
}

void InputManager::connectGamepad(SDL_JoystickID id) {
    SDL_Log("Players possible: %li", this->player_managers->size());
    for(PlayerManager* player : *this->player_managers) {
        SDL_Log("connected: %i", player->isConnected());
        if (!player->isConnected()) {
            player->connect(id);
            return;
        }
    }

    // If no connection was made, overwrite the player using the keyboard
    for(PlayerManager* player : *this->player_managers) {
        if (player->getGamepadId() == KEYBOARD_ID) {
            player->connect(id);
            return;
        }
    }

    SDL_Log("Could not connect gamepad with id %i. There are already 4 connected gamepads.", id);
}

void InputManager::disconnectGamepad() {
    bool keyboard_player_found = false;
    for(PlayerManager* player : *this->player_managers) {
        if(!player->isConnected()) {
            player->disconnect();
        } else {
            if (player->getGamepadId() == KEYBOARD_ID) {
                keyboard_player_found = true;
            }
        }
    }

    // Connect keyboard to first player manager without gamepad
    if (!keyboard_player_found) {
        for(PlayerManager* player : *this->player_managers) {
            if(!player->isConnected()) {
                player->connect(KEYBOARD_ID);
                break;
            }
        }
    }
}
