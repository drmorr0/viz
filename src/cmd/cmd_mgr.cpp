/*
 * David R. Morrison, March 2014
 *
 * List of commands supported by the Viz command prompt, along with a help string and a pointer to
 * the function that should be called when the command is entered
 */

#include "cmd_mgr.h"

using namespace std;

// Clean up the commands that have been registered with the command manager
CommandManager::~CommandManager()
{
	for (auto i = mpRegisteredCommands.begin(); i != mpRegisteredCommands.end(); ++i)
		delete i->second;
}

// Add a new command into the list of registered commands, and specify where that
// command should output messages to
void CommandManager::registerCommand(const string& name, const Command& cmd, Outputter* output)
{
	Command* copy = cmd.clone();
	copy->setOutput(output);
	mpRegisteredCommands[name] = copy;
}

// Unregister a command from the manager; since we own it, need to delete the used memory
void CommandManager::unregisterCommand(const string& name)
{
	if (mpRegisteredCommands.count(name) < 1)
		return;
	delete mpRegisteredCommands[name];
	mpRegisteredCommands.erase(name);
}

// Get a command by name, if it exists, otherwise return nullptr
Command* CommandManager::get(const string& name)
{
	if (mpRegisteredCommands.count(name) < 1)
		return nullptr;
	else return mpRegisteredCommands[name];
}

// Get a space-separated string of the names of all registered commands in the manager
string CommandManager::getRegisteredNames()
{
	string names = "";
	for (auto i = mpRegisteredCommands.begin(); i != mpRegisteredCommands.end(); ++i)
		names += i->first + " ";
	return names;
}
