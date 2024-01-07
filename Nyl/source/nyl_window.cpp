//#include "nyl_window.h"
//
//NylWindow::NylWindow(int width, int height, const std::string& title)
//    : window(nullptr), width(width), height(height), title(title) {}
//
//NylWindow::~NylWindow() {
//    glfwDestroyWindow(window);
//    glfwTerminate();
//}
//
//bool NylWindow::init() {
//    if (!glfwInit()) {
//        return false;
//    }
//
//    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
//    if (!window) {
//        glfwTerminate();
//        return false;
//    }
//
//    glfwMakeContextCurrent(window);
//    return true;
//}
//
//void NylWindow::update() {
//    glfwSwapBuffers(window);
//    glfwPollEvents();
//}
//
//bool NylWindow::shouldClose() const {
//    return glfwWindowShouldClose(window);
//}
