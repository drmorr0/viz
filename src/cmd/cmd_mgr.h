#ifndef CMD_MGR_H
#define CMD_MGR_H
/*
 * cmd_mgr.h: David R. Morrison, March 2014
 *
 * The CommandManager class is a way of registering commands with appropriate subsystems so that the
 * subsystem can quickly and easily identify which commands are run, which commands are supported,
 * etc.  Eventually the manager will be useful in doing things like tab-completion, correcting
 * mistyped commands, and other "useful" features
 *
 */

#include "cmd.h"
#include "types.h"

#include <string>
#include <map>

class CommandManager
{
public:
	CommandManager() { }
	~CommandManager();

	void registerCommand(const std::string& cmdname, const Command& cmd, Outputter* output);
	void unregisterCommand(const std::string& name);
	Command* get(const std::string& name);
	std::string getRegisteredNames();

private:
	std::map<std::string, Command*> mRegisteredCommands;

	CommandManager(CommandManager&) = delete;
	CommandManager& operator=(CommandManager&) = delete;
};

#endif // CMD_MGR_H
