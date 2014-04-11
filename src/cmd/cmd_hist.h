#ifndef CMD_HIST_H
#define CMD_HIST_H
/*
 * cmd_hist.h: David R. Morrison, April 2014
 *
 * The CommandHistory class keeps track of commands that have been previously entered so they can be
 * replayed or undone.
 */

#include "cmd.h"

#include <list>

typedef std::list<Command*>::iterator CommandIter;

class CommandHistory
{
public:
	CommandHistory();
	~CommandHistory();

	void push(const Command& cmd);
	Command* get() const; 
	void step(int stepSize); 

private:
	const int mMaxHistSize;
	std::list<Command*> mpCommands;
	CommandIter mCurrCmd;

	CommandHistory(const CommandHistory&) = delete;
	CommandHistory& operator=(const CommandHistory&) = delete;
};

#endif // CMD_HIST_H
