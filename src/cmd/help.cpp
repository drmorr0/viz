/*
 * David R. Morrison, March 2014
 */

#include "help.h"
#include "viz_prompt.h"

#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

/*
 * Display a general help string, or provide more input for a specific command
 */
HelpCommand::HelpCommand() :
	Command("Display help mesage")
{
	/* Do nothing */
}

bool HelpCommand::operator()(tok_iter& token, const tok_iter& end)
{
	// If we didn't request help on a specific command, display a generic help message
	if (token == end)
	{
		VizPrompt::displayMessage(string("The following commands are recognized by Viz") +
					   string(" (type help command for more info):"), VizPrompt::Info);
		VizPrompt::displayMessage(string("    ") + CommandManager::getRegisteredNames());
	}
	else
	{
		// Find the command that we requested help for
		string cmdStr = trim_copy(*token++);
		auto cmd = CommandManager::get(cmdStr);
		if (cmd) VizPrompt::displayMessage(cmd->help(), VizPrompt::Info);
		else VizPrompt::displayError(string("Unknown command: ") + cmdStr);
	}

	return true;
}

