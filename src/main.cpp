#include <iostream>

#include "cmainwindow.h"

int main (int argc, char* argv[])
{
	Gtk::Main kit(argc, argv);
	CMainWindow mainWindow;

	Gtk::Main::run(mainWindow);
	
	return 0;
}
