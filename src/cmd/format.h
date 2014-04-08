#ifndef FORMAT_H
#define FORMAT_H
/*
 * David R. Morrison, April 2014
 * 
 * Stylize nodes matching the filter according to the given style specification
 *   format Property Operator Value Color [Radius] [Thickness]
 * where Property, Operator, and Value are the same as for filter.  Color is an  HTML color
 * specification (#ff0000), and Radius and Thickness are optional numeric arguments that specify
 * the radius and thickness of the matched vertices.
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
		const Gdk::Color& color, const Gdk::Color& fill, double radius, double thickness);
};

#endif // FORMAT_H
