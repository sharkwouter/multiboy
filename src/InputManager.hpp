#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <SDL.h>
#include <vector>

#include "Input.hpp"
#include "PlayerManager.hpp"

class InputManager {
private:
    SDL_Event event;

    std::vector<PlayerManager*> * player_managers = nullptr;

    bool quit_triggered = false;
    bool controller_event_triggered = false;

    bool joystick_left = true;
    bool joystick_right = true;
    bool joystick_up = true;
    bool joystick_down = true;

    Input getGamepadInput(SDL_ControllerButtonEvent event, bool released);
    Input getGamepadAxisInput(SDL_ControllerAxisEvent event);
    Input getKeyboardInput(SDL_Keycode event, bool released);

    void connectGamepad(SDL_JoystickID id);
    void disconnectGamepad();
public:
    InputManager(std::vector<PlayerManager *> * player_managers);
    ~InputManager();

    void getInputs(std::vector<Input> * inputs);

    bool has_quit_triggered() {return quit_triggered;}
    bool has_controller_event_triggered() {return controller_event_triggered;}
};

#endif // INPUTMANAGER_HPP