#include "input.h"
using namespace nyl;

// ------------------------------ Input ------------------------------------

GLFWwindow* Input::s_window = nullptr;
std::array<bool, Input::kKeyCount>    Input::s_keys{};
std::array<bool, Input::kKeyCount>    Input::s_keysPrev{};
std::array<bool, Input::kButtonCount> Input::s_buttons{};
std::array<bool, Input::kButtonCount> Input::s_buttonsPrev{};
glm::vec2 Input::s_mousePos{ 0.0f };
glm::vec2 Input::s_mousePrev{ 0.0f };
float     Input::s_scrollAccum = 0.0f;
float     Input::s_scrollDelta = 0.0f;

void Input::Init(GLFWwindow* window)
{
    s_window = window;
    s_keys.fill(false);
    s_keysPrev.fill(false);
    s_buttons.fill(false);
    s_buttonsPrev.fill(false);
}

void Input::Update()
{
    // snapshot the previous frame so edge queries work, then publish scroll
    s_keysPrev = s_keys;
    s_buttonsPrev = s_buttons;
    s_mousePrev = s_mousePos;
    s_scrollDelta = s_scrollAccum;
    s_scrollAccum = 0.0f;
}

bool Input::isKeyDown(int key)
{
    return key >= 0 && key < kKeyCount && s_keys[key];
}

bool Input::wasKeyPressed(int key)
{
    return key >= 0 && key < kKeyCount && s_keys[key] && !s_keysPrev[key];
}

bool Input::wasKeyReleased(int key)
{
    return key >= 0 && key < kKeyCount && !s_keys[key] && s_keysPrev[key];
}

glm::vec2 Input::mousePosition() { return s_mousePos; }
glm::vec2 Input::mouseDelta()    { return s_mousePos - s_mousePrev; }
float     Input::scrollDelta()   { return s_scrollDelta; }

bool Input::isMouseButtonDown(int button)
{
    return button >= 0 && button < kButtonCount && s_buttons[button];
}

bool Input::wasMouseButtonPressed(int button)
{
    return button >= 0 && button < kButtonCount && s_buttons[button] && !s_buttonsPrev[button];
}

void Input::OnKey(int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key < 0 || key >= kKeyCount) return;
    if (action == GLFW_PRESS)        s_keys[key] = true;
    else if (action == GLFW_RELEASE) s_keys[key] = false;
    // GLFW_REPEAT leaves the key held
}

void Input::OnMouseButton(int button, int action, int /*mods*/)
{
    if (button < 0 || button >= kButtonCount) return;
    if (action == GLFW_PRESS)        s_buttons[button] = true;
    else if (action == GLFW_RELEASE) s_buttons[button] = false;
}

void Input::OnCursorPos(double x, double y)
{
    s_mousePos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

void Input::OnScroll(double /*xoffset*/, double yoffset)
{
    s_scrollAccum += static_cast<float>(yoffset);
}

// ----------------------------- Joystick ----------------------------------

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