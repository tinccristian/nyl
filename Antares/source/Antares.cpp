#include <Nyl.h>
using namespace Nyl;

int width = 800;
int height = 600;
std::string title = "Antares";

class Antares : public Nyl::Application
{
public:
	Antares()
		: Application(width, height, title)
	{
		NYL_TRACE("ANTARES constructor");
	}
	~Antares()
	{
		NYL_TRACE("ANTARES destructor");
	}

};
//void Nyl::Init()
//{
//	NYL_TRACE("ANTARES init");
//}
//void Nyl::Update()
//{
//	NYL_TRACE("ANTARES update");
//}
Nyl::Application* Nyl::CreateApplication()
{
	NYL_TRACE("Create Antares");
	return new Antares();
}
