// cmd_hist.cpp: David R. Morrison, April 2014
// Implementation details for the command history class

#include "cmd_hist.h"
#include "cmd.h"

CommandHistory::CommandHistory() :
	mMaxHistSize(50),	// TODO - user preference for history size
	mCurrCmd(mpCommands.end())
{
	/* Do nothing */
}

// Need to clean up all of the stored commands
CommandHistory::~CommandHistory()
{
	for (auto i = mpCommands.begin(); i != mpCommands.end(); ++i)
		delete *i;
}

// Add a new command into the history
void CommandHistory::push(const Command& cmd)
{
	mpCommands.push_back(cmd.clone());

	// If we've exceeded the maximum command size, delete the thing at the bottom
	if (mpCommands.size() > mMaxHistSize)
	{
		delete mpCommands.front();
		mpCommands.pop_front();
	}
	
	// Any time a new command is added, we automatically reset our pointer to the end;
	// This way, the next time we start scrolling through the history, we get the most recently
	// entered command first
	mCurrCmd = mpCommands.end();
}

// Get the command currently pointed at
Command* CommandHistory::get() const
{ 
	if (mCurrCmd != mpCommands.end()) return *mCurrCmd;
	else return nullptr;
}

// Adjust the position in the command history up or down
void CommandHistory::step(int stepSize) 
{ 
	int absStepSize = abs(stepSize);
	for (int i = 0; i < absStepSize; ++i)
	{
		if (stepSize < 0 && mCurrCmd != mpCommands.begin()) --mCurrCmd;
		else if (stepSize > 0 && mCurrCmd != mpCommands.end()) ++mCurrCmd;
	}
}

