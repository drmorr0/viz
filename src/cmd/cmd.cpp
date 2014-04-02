/*
 * David R. Morrison, March 2014
 *
 * List of commands supported by the Viz command prompt, along with a help string and a pointer to
 * the function that should be called when the command is entered
 */

#include "cmd.h"

using namespace std;

Command::~Command() { }

map<string, Command*> CommandManager::mRegisteredCommands;

void CommandManager::registerCommand(const string& name, const Command& cmd)
{
	mRegisteredCommands[name] = cmd.clone();
}

void CommandManager::unregisterCommand(const string& name)
{
	if (mRegisteredCommands.count(name) < 1)
		return;
	delete mRegisteredCommands[name];
	mRegisteredCommands.erase(name);
}

Command* CommandManager::get(const string& name)
{
	if (mRegisteredCommands.count(name) < 1)
		return nullptr;
	else return mRegisteredCommands[name];
}

string CommandManager::getRegisteredNames()
{
	string names = "";
	for (auto i = mRegisteredCommands.begin(); i != mRegisteredCommands.end(); ++i)
		names += i->first + " ";
	return names;
}
