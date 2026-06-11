 #pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

#include "core.h"

// analog input	button values					//		PS		|		XBOX
#define GLFW_JOYSTICK_BTN_LEFT 0				//	Square		|	X
#define GLFW_JOYSTICK_BTN_DOWN 1				//	X			|	A
#define GLFW_JOYSTICK_BTN_RIGHT 2				//	Circle		|	B
#define GLFW_JOYSTICK_BTN_UP 3					//	Triangle	|	Y	
#define GLFW_JOYSTICK_SHOULDER_LEFT 4			//	L1			|	LB
#define GLFW_JOYSTICK_SHOULDER_RIGHT 5			//	R1			|	RB
#define GLFW_JOYSTICK_TRIGGER_LEFT 6			//	L2			|	LT
#define GLFW_JOYSTICK_TRIGGER_RIGHT 7			//	R2			|	RT
#define GLFW_JOYSTICK_SELECT 8					//	Share		|	Address
#define GLFW_JOYSTICK_START 9					//	Options		|	Menu
#define GLFW_JOYSTICK_LEFT_STICK 10				//	L3			|	LS
#define GLFW_JOYSTICK_RIGHT_STICK 11			//	R3			|	RS
#define GLFW_JOYSTICK_HOME 12					//	Home		|	Home
#define GLFW_JOYSTICK_CLICK 13					//	Touch pad	|	n/a
#define GLFW_JOYSTICK_DPAD_UP 14				//	Dpad up		|	Dpad up
#define GLFW_JOYSTICK_DPAD_RIGHT 15				//	Dpad right	|	Dpad right
#define GLFW_JOYSTICK_DPAD_DOWN 16				//	Dpad down	|	Dpad down
#define GLFW_JOYSTICK_DPAD_LEFT 17				//	Dpad left	|	Dpad left

// axes
#define GLFW_JOYSTICK_AXES_LEFT_STICK_X 0
#define GLFW_JOYSTICK_AXES_LEFT_STICK_Y 1
#define GLFW_JOYSTICK_AXES_RIGHT_STICK_X 2
#define GLFW_JOYSTICK_AXES_LEFT_TRIGGER 3
#define GLFW_JOYSTICK_AXES_RIGHT_TRIGGER 4
#define GLFW_JOYSTICK_AXES_RIGHT_STICK_Y 5

namespace nyl
{
    /**
     * @brief Polling keyboard + mouse input with per-frame edge detection.
     *
     * The Window feeds GLFW callbacks into the On* handlers; the engine calls
     * Update() once at the very start of each frame (before glfwPollEvents) to
     * snapshot the previous state, which is what makes wasKeyPressed() /
     * wasKeyReleased() one-shot edge queries possible.
     *
     * Use key codes from GLFW (e.g. GLFW_KEY_A, GLFW_KEY_SPACE) and mouse
     * buttons (e.g. GLFW_MOUSE_BUTTON_LEFT).
     */
    class NYL_API Input
    {
    public:
        static void Init(GLFWwindow* window);
        static void Update(); // call once per frame, before glfwPollEvents()

        // keyboard
        static bool isKeyDown(int key);
        static bool wasKeyPressed(int key);
        static bool wasKeyReleased(int key);

        // mouse
        static glm::vec2 mousePosition();
        static glm::vec2 mouseDelta();
        static float     scrollDelta();
        static bool      isMouseButtonDown(int button);
        static bool      wasMouseButtonPressed(int button);

        // fed by Window's GLFW callbacks
        static void OnKey(int key, int scancode, int action, int mods);
        static void OnMouseButton(int button, int action, int mods);
        static void OnCursorPos(double x, double y);
        static void OnScroll(double xoffset, double yoffset);

    private:
        static constexpr int kKeyCount = GLFW_KEY_LAST + 1;
        static constexpr int kButtonCount = GLFW_MOUSE_BUTTON_LAST + 1;

        static GLFWwindow* s_window;
        static std::array<bool, kKeyCount>    s_keys;
        static std::array<bool, kKeyCount>    s_keysPrev;
        static std::array<bool, kButtonCount> s_buttons;
        static std::array<bool, kButtonCount> s_buttonsPrev;
        static glm::vec2 s_mousePos;
        static glm::vec2 s_mousePrev;
        static float     s_scrollAccum; // accumulated since last Update
        static float     s_scrollDelta; // scroll during the last frame
    };
}

/*
    joystick class to handle input from joystick controller
*/
namespace nyl{
class NYL_API Joystick {
public:
    /*
        constructor
    */

    // generate an instance for joystick with id i
    Joystick(int i);

    // update the joystick's states
    void update();

    /*
        accessors
    */

    // get axis value
    float axesState(int axis);
    // get button state
    unsigned char buttonState(int button);

    // get number of axes
    int getAxesCount();
    // get number of buttons
    int getButtonCount();

    // return if joystick present
    bool isPresent();
    // get name of joystick
    const char* getName();

    // static method to get enum value for joystick
    static int getId(int i);

private:
    /*
        joystick values
    */

    // 1 if present, 0 if not
    int present;

    // joystick id
    int id;

    // joystick name
    const char* name;

    // number of axes on joystick
    int axesCount;
    // array of axes values
    const float* axes;

    // number of buttons
    int buttonCount;
    // array of button states
    const unsigned char* buttons;
};
}