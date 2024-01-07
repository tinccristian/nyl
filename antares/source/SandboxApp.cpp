//#include "nyl_window.h"
//
//int main() {
//    NylWindow window(800, 600, "OpenGL Window");
//
//    if (!window.init()) {
//        // Handle initialization failure
//        return -1;
//    }
//
//    while (!window.shouldClose()) {
//        // Clear the framebuffer
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Rendering commands here
//
//        window.update();
//    }
//
//    return 0;
//}
#include <Nyl.h>

class Antares : public Nyl::Application
{
public:
	Antares()
	{

	}

	~Antares()
	{

	}
};

Nyl::Application* Nyl::CreateApplication()
{
	return new Antares();
}