// cmd_hist.cpp: David R. Morrison, April 2014
// Implementation details for the command history class

#include "cmd_hist.h"
#include "cmd.h"

CommandHistory::CommandHistory() :
	mMaxHistSize(50),	// TODO
	mCurrCmd(mpCommands.end())
{
	/* Do nothing */
}

CommandHistory::~CommandHistory()
{
	for (auto i = mpCommands.begin(); i != mpCommands.end(); ++i)
		delete *i;
}

void CommandHistory::push(const Command* const cmd)
{
	mpCommands.back();
	mpCommands.push_back(cmd->clone());
	if (mpCommands.size() > mMaxHistSize)
	{
		delete mpCommands.front();
		mpCommands.pop_front();
	}
	mCurrCmd = mpCommands.end();
}

Command* CommandHistory::get() const
{ 
	if (mCurrCmd != mpCommands.end()) return *mCurrCmd;
	else return nullptr;
}

void CommandHistory::step(int stepSize) 
{ 
	int absStepSize = abs(stepSize);
	for (int i = 0; i < absStepSize; ++i)
	{
		if (stepSize < 0 && mCurrCmd != mpCommands.begin()) --mCurrCmd;
		else if (stepSize > 0 && mCurrCmd != mpCommands.end()) ++mCurrCmd;
	}
}

