#ifndef INPUT_HPP
#define INPUT_HPP

#include "InputType.hpp"

typedef struct {
    int gamepad_id;
    InputType type;
    bool released;
} Input;

#endif // INPUT_HPP