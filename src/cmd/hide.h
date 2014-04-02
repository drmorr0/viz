/*
 * David R. Morrison, April 2014
 */

#include "cmd.h"
#include "types.h"

class HideCommand : public Command
{
public:
	HideCommand();
	Command* clone() const { return new HideCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);
	bool operator()(const std::string& filterBy, MatchOp op, double value);
};
