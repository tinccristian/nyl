 #pragma once

#include <cstdint>
#include <GLFW/glfw3.h>


// Input Structs
enum KeyCodeID
{
	KEY_MOUSE_LEFT,
	KEY_MOUSE_MIDDLE,
	KEY_MOUSE_RIGHT,

	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
	KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
	KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

	KEY_SPACE,
	KEY_TICK,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_LEFT_BRACKET,
	KEY_RIGHT_BRACKET,
	KEY_SEMICOLON,
	KEY_QUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_FORWARD_SLASH,
	KEY_BACKWARD_SLASH,
	KEY_TAB,
	KEY_ESCAPE,
	KEY_PAUSE,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_BACKSPACE,
	KEY_RETURN,
	KEY_DELETE,
	KEY_INSERT,
	KEY_HOME,
	KEY_END,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_CAPS_LOCK,
	KEY_NUM_LOCK,
	KEY_SCROLL_LOCK,
	KEY_MENU,
	KEY_SHIFT,
	KEY_CONTROL,
	KEY_ALT,
	KEY_COMMAND,

	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
	KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

	KEY_NUMPAD_0,
	KEY_NUMPAD_1,
	KEY_NUMPAD_2,
	KEY_NUMPAD_3,
	KEY_NUMPAD_4,
	KEY_NUMPAD_5,
	KEY_NUMPAD_6,
	KEY_NUMPAD_7,
	KEY_NUMPAD_8,
	KEY_NUMPAD_9,

	KEY_NUMPAD_STAR,
	KEY_NUMPAD_PLUS,
	KEY_NUMPAD_MINUS,
	KEY_NUMPAD_DOT,
	KEY_NUMPAD_SLASH,

	KEY_COUNT = 255,
};

struct Key
{
	bool isDown; // indicates if the key is currently pressed
	bool justPressed; // indicates if the key was just pressed
	bool justReleased; // indicates if the key was just released
	unsigned char halfTransitionCount; // counts the number of half transitions (press and release) of the key
};

struct IVec2
{
	int x;
	int y;

	IVec2 operator-(IVec2 other)
	{
		return { x - other.x, y - other.y };
	}

	IVec2& operator-=(int value)
	{
		x -= value;
		y -= value;
		return *this;
	}

	IVec2& operator+=(int value)
	{
		x += value;
		y += value;
		return *this;
	}

	IVec2 operator/(int scalar)
	{
		return { x / scalar, y / scalar };
	}
};

struct Input
{
	IVec2 screenSize; // size of the screen

	// screen
	IVec2 prevMousePos; // previous mouse position on the screen
	IVec2 mousePos; // current mouse position on the screen
	IVec2 relMouse; // relative mouse movement

	// world
	IVec2 prevMousePosWorld; // previous mouse position in the world
	IVec2 mousePosWorld; // current mouse position in the world
	IVec2 relMouseWorld; // relative mouse movement in the world

	Key keys[KEY_COUNT]; // array of keys
};

// input Globals
//static Input* input; // global input object

// input Functions

// // check if a key is currently pressed
// bool _isKeyPressed(KeyCodeID keyCode)
// {
// 	Key key = input->keys[keyCode];
// 	bool result = key.isDown && (key.halfTransitionCount == 1 || key.halfTransitionCount > 1);
// 	return result;
// }

// // check if a key was just released
// bool _isKeyReleased(KeyCodeID keyCode)
// {
// 	Key key = input->keys[keyCode];
// 	bool result = !key.isDown && (key.halfTransitionCount == 1 || key.halfTransitionCount > 1);
// 	return result;
// }

// // check if a key is currently down
// bool _isKeyDown(KeyCodeID keyCode)
// {
// 	return input->keys[keyCode].isDown;
// }

enum JoystickID
{
    JOYSTICK_1, JOYSTICK_2, JOYSTICK_3, JOYSTICK_4,
    JOYSTICK_5, JOYSTICK_6, JOYSTICK_7, JOYSTICK_8,
    JOYSTICK_9, JOYSTICK_10, JOYSTICK_11, JOYSTICK_12,
    JOYSTICK_13, JOYSTICK_14, JOYSTICK_15, JOYSTICK_16
};

enum JoystickButtonID
{
    BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4,
    BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8,
    BUTTON_9, BUTTON_10, BUTTON_11, BUTTON_12,
    BUTTON_13, BUTTON_14, BUTTON_15
};

enum JoystickAxisID
{
    AXIS_1, AXIS_2, AXIS_3, AXIS_4,
    AXIS_5, AXIS_6//, AXIS_7, AXIS_8
};

class JoystickManager
{
public:
    static bool IsJoystickPresent(int joystickID)
    {
        GLint internalID =joystickID;
        return glfwJoystickPresent(internalID);
    }

    static std::string GetJoystickName(int joystickID)
    {
        const char* name = glfwGetJoystickName(joystickID);
        return (name != nullptr) ? std::string(name) : std::string();
    }

    static std::vector<float> GetJoystickAxes(int joystickID)
    {
        int count;
        const float* axes = glfwGetJoystickAxes(joystickID, &count);
        return std::vector<float>(axes, axes + count);
    }

    static std::vector<unsigned char> GetJoystickButtons(int joystickID)
    {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(joystickID, &count);
        return std::vector<unsigned char>(buttons, buttons + count);
    }

    // Add more joystick input handling functions as needed

private:
    JoystickManager() = delete;
    ~JoystickManager() = delete;
};
//     {
// class JoystickInput
// {
// public:
// 	// Check if a joystick is connected
// 	static bool IsJoystickConnected()
// 	{
//             //    joystick = glfwJoystickPresent(GLFW_JOYSTICK_1);

//         auto joystick1 = glfwJoystickPresent(GLFW_JOYSTICK_1);
//         auto joystick2 = glfwJoystickPresent(GLFW_JOYSTICK_2);
//         auto joystick3 = glfwJoystickPresent(GLFW_JOYSTICK_3);
//         auto joystick4 = glfwJoystickPresent(GLFW_JOYSTICK_4);
//        //axes ~ should be 6 for regular controller: leftAnalog axis x,y, rightAnalog axis x,y, left/right triggers (LT) 1 axis each
//        int axesCount;
//        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
//        NYL_CORE_INFO("#axes available: {0}", axesCount);

//        // buttons
//         const unsigned char* buttons;
//        int buttonCount;
//        buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

//        //info
//        NYL_CORE_INFO("{0} is connected, it has {1} axes available, {2} buttons", glfwGetJoystickName(GLFW_JOYSTICK_1), axesCount,buttonCount);


//            //joystick
//            if (1 == joystick1)
//            {
//                NYL_CORE_TRACE("\n\n\n\n\n\n\n\n\n\n\n\n\n\nLeft stick X Axis: {0}", axes[0]);
//                NYL_CORE_TRACE("Left stick Y Axis: {0}", axes[1]);
//                NYL_CORE_TRACE("Right stick X Axis: {0}", axes[2]);
//                NYL_CORE_TRACE("Right stick Y Axis: {0}", axes[5]);
//                NYL_CORE_TRACE("Left Trigger/L2: {0}", axes[4]);
//                NYL_CORE_TRACE("Right Trigger/R2: {0}", axes[3]);


//                if (GLFW_PRESS == buttons[1])
//                {
//                    NYL_CORE_TRACE("X button pressed");
//                }
// 	        }
// 		return joystick1;
//     }

// 	// Update gamepad mappings
// 	static void UpdateGamepadMappings(const std::string& mappings)
// 	{
// 		glfwUpdateGamepadMappings(mappings.c_str());
// 	}

// 	// Get the axes values of the joystick
// 	static std::vector<float> GetJoystickAxes()
// 	{
// 		int axesCount;
// 		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
// 		return std::vector<float>(axes, axes + axesCount);
// 	}

// 	// Get the button states of the joystick
// 	static std::vector<int> GetJoystickButtons()
// 	{
// 		int buttonCount;
// 		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
// 		std::vector<int> buttonStates(buttons, buttons + buttonCount);
// 		return buttonStates;
// 	}

// 	// Get the name of the joystick
// 	static std::string GetJoystickName()
// 	{
// 		return glfwGetJoystickName(GLFW_JOYSTICK_1);
// 	}
// };

