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