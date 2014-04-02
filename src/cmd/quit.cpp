/*
 * David R. Morrison, March 2014
 */

#include "quit.h"

#include <gtkmm.h>

QuitCommand::QuitCommand() :
	Command("Quit Viz")
{
	/* Do nothing */
}

bool QuitCommand::operator()(tok_iter& token, const tok_iter& end)
{
    Gtk::Main::quit();
    return true;
}



