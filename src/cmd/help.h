#ifndef HELP_H
#define HELP_H
/*
 * David R. Morrison, April 2014
 *
 * Display a general help string, or provide more input for a specific command
 */

#include "cmd.h"

class HelpCommand : public Command
{
public:
	HelpCommand();
	Command* clone() const { return new HelpCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);
};

#endif // HELP_H
