/*
 * David R. Morrison, March 2014
 *
 * List of commands supported by the Viz command prompt, along with a help string and a pointer to
 * the function that should be called when the command is entered
 */

#include "cmd_mgr.h"

using namespace std;

CommandManager::~CommandManager()
{
	for (auto i = mRegisteredCommands.begin(); i != mRegisteredCommands.end(); ++i)
		delete i->second;
}

void CommandManager::registerCommand(const string& name, const Command& cmd, Outputter* output)
{
	Command* copy = cmd.clone();
	copy->setOutput(output);
	mRegisteredCommands[name] = copy;
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
