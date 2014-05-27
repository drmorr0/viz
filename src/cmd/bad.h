#ifndef BAD_H
#define BAD_H
/*
 * David R. Morrison, April 2014
 *
 * An invalid or bad command
 */

#include "cmd.h"

class BadCommand : public Command
{
public:
	BadCommand(const string& cmdStr) : Command("bad", "bad", {{}}) { mInputStr = cmdStr; }
	Command* clone() const { return new BadCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end) { return false; }
};

#endif // BAD_H
