#ifndef CMD_H
#define CMD_H
/*
 * cmd.h: David R. Morrison, March 2014
 *
 * The command interface is 
 * a collection of functions and classes that are useful for inputting commands into Viz.
 * Since the same command can be given a variety of different ways (through the command intepreter,
 * via context menus, etc.), the commands are represented as functor objects which are stored in a
 * CommandManager class
 *
 * The important thing to note is that input commands are sequentially applied.  So, for example,
 * filtering all of the nodes with lower bound <= 5 and then filtering all * of the nodes with lower
 * bound > 3 will only show the nodes with lower bound in (3, 5].  The same is true for node
 * formatting.  This is desired behavior and future updates should make sure they operate in the
 * same way
 *
 */

#include "outputter.h"
#include "types.h"

#include <string>
#include <map>

class VizPrompt;

class Command
{
public:
	virtual ~Command() = 0;
	virtual Command* clone() const = 0;
	
	std::string help() const { return mHelpString; }
	void setOutput(Outputter* output) { fpOutput = output; }
	virtual bool operator()(tok_iter& token, const tok_iter& end) = 0;

protected:
	Command(const std::string& help) : mHelpString(help) { }

	const std::string mHelpString;
	Outputter* fpOutput;
};

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

#endif // CMD_H
