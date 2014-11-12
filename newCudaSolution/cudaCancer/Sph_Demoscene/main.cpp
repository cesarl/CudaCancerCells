#include "App.hpp"
#undef main

int main(int argc, char **argv)
{
	App app;
	app.init();
	while (app.run())
	{ }
	app.deactivate();
	return EXIT_SUCCESS;
}