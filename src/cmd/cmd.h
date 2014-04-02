#ifndef CMD_H
#define CMD_H
/*
 * cmd.h: David R. Morrison, March 2014
 *
 * This is a collection of functions and classes that are useful for inputting commands into Viz.
 * Since the same command can be given a variety of different ways (through the command intepreter,
 * via context menus, etc.) this also provides a set of wrapper functions for running the various
 * commands in different ways.
 *
 * The important thing to note is that input commands are sequentially applied.  So, for example,
 * filtering all of the nodes with lower bound <= 5 and then filtering all * of the nodes with lower
 * bound > 3 will only show the nodes with lower bound in (3, 5].  The same is true for node
 * formatting.  This is desired behavior and future updates should make sure they operate in the
 * same way
 *
 * Eventually, we want to be able to read a script from a file, so that we can quickly and easily
 * format lots of different graphs the same way TODO
 
 */

#include <string>
#include <map>

// Sigh... boost...
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::escaped_list_separator<char>>::iterator tok_iter;

class Command
{
public:
	virtual ~Command() = 0;
	virtual Command* clone() const = 0;
	
	std::string help() const { return mHelpString; }
	virtual bool operator()(tok_iter& token, const tok_iter& end) = 0;

protected:
	Command(const std::string& help) : mHelpString(help) { } ;
	const std::string mHelpString;
};

class CommandManager
{
public:
	static void registerCommand(const std::string& cmdname, const Command& cmd);
	static void unregisterCommand(const std::string& name);
	static Command* get(const std::string& name);
	static std::string getRegisteredNames();

private:
	static std::map<std::string, Command*> mRegisteredCommands;

	CommandManager() = delete;
	CommandManager(CommandManager&) = delete;
	CommandManager& operator=(CommandManager&) = delete;
};

#endif // CMD_H
