/*
 * David R. Morrison, March 2014
 */

#include "help.h"
#include "cmd_mgr.h"

#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

HelpCommand::HelpCommand(CommandManager* cmdMgr) :
	Command("help", "Display help mesage", {{ /* No subcommands */ }}),
	fpCmdMgr(cmdMgr)
{
	/* Do nothing */
}

bool HelpCommand::operator()(tok_iter& token, const tok_iter& end)
{
	if (token == end) 
	{
		fpOutput->writeInfo("The following commands are recognized by Viz"  
				" (type help <command> for more info):");
		fpOutput->write(string("    ") + fpCmdMgr->getRegisteredNames());
	}

	else 
	{
		string cmdName = trim_copy(*token++);
		auto cmd = fpCmdMgr->get(cmdName);
		if (cmd) fpOutput->writeInfo(cmd->help(token, end));
		else fpOutput->writeError(string("Unknown command: ") + cmdName);
	}

	return true;
}

