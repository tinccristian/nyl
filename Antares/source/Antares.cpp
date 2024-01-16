// antares.cpp
#include "../../Antares/Antares.h"
#include <header/EntityManager.h>

namespace Antares
{

    Antares::Antares(int width, int height, const std::string& title)
        : Nyl::Application(width, height, title)
    {
        NYL_TRACE("ANTARES constructor");
    }

    Antares::~Antares()
    {
        NYL_TRACE("ANTARES destructor");
    }

    void Antares::Init()
    {
        NYL_TRACE("ANTARES init");

    }

    void Antares::Update()
    {
        //NYL_TRACE("ANTARES update");
    }

    void Antares::Quit()
    {
        NYL_TRACE("ANTARES quit");
    }

}
Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(800, 400, "Antares");
}