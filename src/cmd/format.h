#ifndef FORMAT_H
#define FORMAT_H
/*
 * David R. Morrison, April 2014
 * 
 * Stylize nodes matching the filter according to the given style specification
 */

#include "cmd.h"
#include "cmd_parse.h"
#include "types.h"

#include <string>
#include <gtkmm.h>

class FormatCommand : public Command
{
public:
	FormatCommand();
	~FormatCommand() { }
	Command* clone() const { return new FormatCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);
};

#endif // FORMAT_H
