#include "input.h"
using namespace nyl;

Joystick::Joystick(int i) {
    this->id = getId(i);

    if (this->id != -1) {
        update();
    }
}

void Joystick::update() {
    id = GLFW_JOYSTICK_1;
    present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (present==1) {
        name = glfwGetJoystickName(id);
        axes = glfwGetJoystickAxes(id, &axesCount);
        buttons = glfwGetJoystickButtons(id, &buttonCount);
    }
}

float Joystick::axesState(int axis) {
    if (present && axis >= 0 && axis < axesCount) {
        return axes[axis];
    }

    return -1;
}

unsigned char Joystick::buttonState(int button) {
    if (present && button >= 0 && button < buttonCount) {
        return buttons[button];
    }

    return GLFW_RELEASE;
}

int Joystick::getAxesCount() {
    if (present) {
        return axesCount;
    }

    return 0;
}

int Joystick::getButtonCount() {
    if (present) {
        return buttonCount;
    }

    return 0;
}

bool Joystick::isPresent() {
    return present;
}

const char* Joystick::getName() {
    if (present) {
        return name;
    }

    return nullptr;
}

int Joystick::getId(int i) {
    if (i >= 0 && i <= GLFW_JOYSTICK_LAST - GLFW_JOYSTICK_1) {
        return GLFW_JOYSTICK_1 + i;
    }
    else {
        return -1;
    }
}