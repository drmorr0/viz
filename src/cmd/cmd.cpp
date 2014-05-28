// cmd.cpp: David R. Morrison
// Implementation details for the abstract Command class

#include "cmd.h"
#include "cmd_parse.h"

#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

// Output the help string for a given command
string Command::help(tok_iter& token, const tok_iter& end) const
{
	string helpStr;

	// If we didn't ask for help on a subcommand, then just print out the command name, its
	// description, and the list of accepted subcommands (if any are present)
	if (token == end)
	{
		helpStr += mCmdName + ": " + mCmdDesc;
		if (mSubCommands.size() > 0)
		{
			helpStr += "\n    sub-commands (type help <sub-command> for more info): ";
			for (auto i = mSubCommands.begin(); i != mSubCommands.end(); ++i)
				helpStr += i->first + " ";
		}
	}

	// Otherwise, provide help on the specified subcommand, if it's valid
	else
	{
		string subc = trim_copy(*token++);
		if (mSubCommands.count(subc) > 0 && token == end)
			helpStr += mCmdName + " " + subc + " " + mSubCommands.find(subc)->second->help();
		else
		{
			fpOutput->writeError("Unrecognized subcommand " + subc);
			return "";
		}
	}
	return helpStr;
}
