#ifndef FORMAT_H
#define FORMAT_H
/*
 * David R. Morrison, April 2014
 */

#include "cmd.h"
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
	bool operator()(const std::string& filterBy, MatchOp oper, double value, 
		const Gdk::Color color, double radius, double thickness);
};

#endif // FORMAT_H
