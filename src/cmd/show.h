#ifndef SHOW_H
#define SHOW_H
/*
 * David R. Morrison, April 2014
 */

#include "cmd.h"

class ShowCommand : public Command
{
public:
	ShowCommand();
	Command* clone() const { return new ShowCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);
};

#endif // SHOW_H
