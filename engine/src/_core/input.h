 #pragma once

#include <GLFW/glfw3.h>
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

/*
    joystick class to handle input from joystick controller
*/
namespace nyl{

enum JoystickInput
{
    // According to glfw
    BTN_LEFT,      BTN_DOWN,       BTN_RIGHT,    BTN_UP,         // Square,  X,        Circle, Triangle | X,       A,    B,     Y
    SHOULDER_LEFT, SHOULDER_RIGHT, TRIGGER_LEFT, TRIGGER_RIGHT,  // L1,      R1,       L2,     R2       | LB,      RB,   LT,    RT
    SELECT,        START,          LEFT_STICK,   RIGHT_STICK,    // Share,   Options,  L3,     R3       | Address, Menu, LS,    RS
    HOME,          CLICK,          DPAD_UP,      DPAD_RIGHT,     // Home,    TouchPad, PadUP,  PadRight | Home,    N/A,  PadUP, PadRight 
    DPAD_DOWN,     DPAD_LEFT                                     // PadDown, PadLeft                    | PadDown, PadLeft
};

enum KeyCode
{
	// According to glfw
	Space               = 32,
	Apostrophe          = 39, /* ' */
	Comma               = 44, /* , */
	Minus               = 45, /* - */
	Period              = 46, /* . */
	Slash               = 47, /* / */

	D0                  = 48, /* 0 */
	D1                  = 49, /* 1 */
	D2                  = 50, /* 2 */
	D3                  = 51, /* 3 */
	D4                  = 52, /* 4 */
	D5                  = 53, /* 5 */
	D6                  = 54, /* 6 */
	D7                  = 55, /* 7 */
	D8                  = 56, /* 8 */
	D9                  = 57, /* 9 */

	Semicolon           = 59, /* ; */
	Equal               = 61, /* = */

	A                   = 65,
	B                   = 66,
	C                   = 67,
	D                   = 68,
	E                   = 69,
	F                   = 70,
	G                   = 71,
	H                   = 72,
	I                   = 73,
	J                   = 74,
	K                   = 75,
	L                   = 76,
	M                   = 77,
	N                   = 78,
	O                   = 79,
	P                   = 80,
	Q                   = 81,
	R                   = 82,
	S                   = 83,
	T                   = 84,
	U                   = 85,
	V                   = 86,
	W                   = 87,
	X                   = 88,
	Y                   = 89,
	Z                   = 90,

	LeftBracket         = 91,  /* [ */
	Backslash           = 92,  /* \ */
	RightBracket        = 93,  /* ] */
	GraveAccent         = 96,  /* ` */

	World1              = 161, /* non-US #1 */
	World2              = 162, /* non-US #2 */

	/* Function keys */
	Escape              = 256,
	Enter               = 257,
	Tab                 = 258,
	Backspace           = 259,
	Insert              = 260,
	Delete              = 261,
	Right               = 262,
	Left                = 263,
	Down                = 264,
	Up                  = 265,
	PageUp              = 266,
	PageDown            = 267,
	Home                = 268,
	End                 = 269,
	CapsLock            = 280,
	ScrollLock          = 281,
	NumLock             = 282,
	PrintScreen         = 283,
	Pause               = 284,
	F1                  = 290,
	F2                  = 291,
	F3                  = 292,
	F4                  = 293,
	F5                  = 294,
	F6                  = 295,
	F7                  = 296,
	F8                  = 297,
	F9                  = 298,
	F10                 = 299,
	F11                 = 300,
	F12                 = 301,
	F13                 = 302,
	F14                 = 303,
	F15                 = 304,
	F16                 = 305,
	F17                 = 306,
	F18                 = 307,
	F19                 = 308,
	F20                 = 309,
	F21                 = 310,
	F22                 = 311,
	F23                 = 312,
	F24                 = 313,
	F25                 = 314,

	/* Keypad */
	KP0                 = 320,
	KP1                 = 321,
	KP2                 = 322,
	KP3                 = 323,
	KP4                 = 324,
	KP5                 = 325,
	KP6                 = 326,
	KP7                 = 327,
	KP8                 = 328,
	KP9                 = 329,
	KPDecimal           = 330,
	KPDivide            = 331,
	KPMultiply          = 332,
	KPSubtract          = 333,
	KPAdd               = 334,
	KPEnter             = 335,
	KPEqual             = 336,

	LeftShift           = 340,
	LeftControl         = 341,
	LeftAlt             = 342,
	LeftSuper           = 343,
	RightShift          = 344,
	RightControl        = 345,
	RightAlt            = 346,
	RightSuper          = 347,
	Menu                = 348
};

enum MouseCode
{
	// According to glfw
	Button0                = 0,
	Button1                = 1,
	Button2                = 2,
	Button3                = 3,
	Button4                = 4,
	Button5                = 5,
	Button6                = 6,
	Button7                = 7,
	ButtonLast             = Button7,
	ButtonLeft             = Button0,
	ButtonRight            = Button1,
	ButtonMiddle           = Button2
};

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