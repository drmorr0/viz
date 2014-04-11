#ifndef VIZ_PROMPT_H
#define VIZ_PROMPT_H
/*
 * viz_prompt.h: David R. Morrison, March 2014
 *
 * These functions handle all of the command prompt and output for Viz.  It uses the Boost tokenizer
 * to parse input commands.  
 *
 */

#include "outputter.h"
#include "cmd_hist.h"
#include "util.h"

#include <memory>
#include <string>
#include <gtkmm.h>

class Command;
class CommandManager;

class VizPrompt : public Outputter
{
public:
	VizPrompt(const char* inputName, const char* outputName, const char* scrollName, 
			CommandManager* cmdMgr);

	void read();

	void writeInfo(const string& text); 
	void writeWarning(const string& text); 
	void writeError(const string& text); 
	void write(const string& text, OutputStyle = Normal);
	
private:
	Gtk::Entry* fpInput;
	Gtk::TextView* fpOutput;
	Gtk::ScrolledWindow* fpScrollPane;

	CommandManager* fpCmdMgr;
	std::unique_ptr<CommandHistory> pCmdHist;

	bool navigate(GdkEventKey* key);
	void refreshOutput(Gtk::Allocation& a);

	VizPrompt(const VizPrompt&) = delete;
	VizPrompt& operator=(const VizPrompt&) = delete;
};

#endif // VIZ_PROMPT_H
