#ifndef LOAD_H
#define LOAD_H
/*
 * David R. Morrison, April 2014
 *
 * This command loads a file; there are two types of files that can be loaded:
 *  1) a list of commands (called a script)
 *  2) a new graph (TODO)
 * The syntax for the load command is
 * 	load [graph|script] Filename
 * where Filename is the path from the current directory to the file to load.
 */

#include "cmd.h"

class LoadCommand : public Command
{
public:
	LoadCommand();
	Command* clone() const { return new LoadCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);

private:
	bool loadGraph(const std::string& filename);
	bool loadScript(const std::string& filename);
};

#endif // LOAD_H
