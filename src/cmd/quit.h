#ifndef QUIT_H
#define QUIT_H
/*
 * David R. Morrison, April 2014
 */

#include "cmd.h"

class QuitCommand : public Command
{
public:
	QuitCommand();
	Command* clone() const { return new QuitCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);
};

#endif // QUIT_H
