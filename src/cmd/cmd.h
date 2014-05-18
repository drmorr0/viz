#ifndef CMD_H
#define CMD_H
/*
 * cmd.h: David R. Morrison, March 2014
 *
 * The command interface is a collection of functions and classes that are useful for inputting
 * commands into Viz.  Since the same command can be given a variety of different ways (through the
 * command intepreter, via context menus, etc.), the commands are represented as functor objects
 * which are stored in a CommandManager class
 *
 * The important thing to note is that input commands are sequentially applied.  So, for example,
 * filtering all of the nodes with lower bound <= 5 and then filtering all of the nodes with lower
 * bound > 3 will only show the nodes with lower bound in (3, 5].  The same is true for node
 * formatting.  This is desired behavior and future updates should make sure they operate in the
 * same way
 *
 */

#include "outputter.h"
#include "types.h"

#include <cassert>
#include <string>
#include <tuple>
#include <map>

class Command
{
public:
	virtual ~Command() { }
	virtual Command* clone() const = 0;

	void setInputStr(const std::string& str) { mInputStr = str; }
	std::string getInputStr() { return mInputStr; }
	void setOutput(Outputter* output) { fpOutput = output; }

	virtual std::string help() const = 0;
	virtual bool operator()(tok_iter& token, const tok_iter& end) = 0;

protected:
	Command(const std::string& name, const std::string& help) : mCmdName(name) { }

	const std::string mCmdName;
	std::string mInputStr;
	Outputter* fpOutput;
};

#endif // CMD_H
