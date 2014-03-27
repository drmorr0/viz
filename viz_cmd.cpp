// viz_cmd.cpp: David R. Morrison, March 2014
// Implementation for the viz command prompt

#include "viz_cmd.h"

using namespace std;

VizCmdPrompt::VizCmdPrompt()
{
	TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->signal_activate().connect(
			sigc::mem_fun(*this, &VizCmdPrompt::parseCommand));
}

void VizCmdPrompt::parseCommand()
{
	printf("Activated!\n");
}

