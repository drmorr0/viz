/*
 * David R. Morrison, April 2014
 * Only show nodes matching the specified filter.  The syntax for this command is
 *   filter Property Operator Value
 * where Property is any stored property of the graph, Operator is one of <,<=,==,!=,>=,>, and
 * Value is a numeric constant.
 */

#include "cmd.h"
#include "cmd_parse.h"
#include "types.h"

class HideCommand : public Command
{
public:
	HideCommand();
	Command* clone() const { return new HideCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);
};
