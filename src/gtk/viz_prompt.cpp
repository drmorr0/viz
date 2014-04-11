// viz_prompt.cpp: David R. Morrison, March 2014
// Implementation functions for the viz command prompt

#include "viz_prompt.h"
#include "builder.h"

#include "cmd_mgr.h"
#include "cmd_hist.h"
#include "bad.h"

#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

VizPrompt::VizPrompt(const char* inputName, const char* outputName, const char* scrollName,
		CommandManager* cmdMgr) :
	fpInput(TheBuilder::get<Gtk::Entry>(inputName)),
	fpOutput(TheBuilder::get<Gtk::TextView>(outputName)),
	fpScrollPane(TheBuilder::get<Gtk::ScrolledWindow>(scrollName)),
	fpCmdMgr(cmdMgr),
	pCmdHist(new CommandHistory)
{
	// Set up an event handler when the command prompt has focus and the user presses enter
	fpInput->signal_activate().connect(sigc::mem_fun(*this, &VizPrompt::read));
	fpInput->signal_key_press_event().connect(sigc::mem_fun(*this, &VizPrompt::navigate));

	// Create some formatting styles -- we may want to create these elsewhere if 
	// we plan to use them for multiple buffers (TODO)
	auto errorTag = fpOutput->get_buffer()->create_tag("style_error");
	errorTag->property_weight() = Pango::WEIGHT_BOLD;
	errorTag->property_foreground() = "#ff0000";
	auto infoTag = fpOutput->get_buffer()->create_tag("style_info");
	infoTag->property_weight() = Pango::WEIGHT_BOLD;
	infoTag->property_foreground() = "#0000ff";
	auto warnTag = fpOutput->get_buffer()->create_tag("style_warning");
	warnTag->property_weight() = Pango::WEIGHT_BOLD;
	warnTag->property_foreground() = "#ffa500";

	// When the output window size is allocated, then scroll to the bottom of the output window
	// Necessary because the new size might not be computed until a redraw
	fpOutput->signal_size_allocate().connect(sigc::mem_fun(*this, &VizPrompt::refreshOutput));
}

/*
 * Parse a command that is typed into the command prompt
 */
void VizPrompt::read()
{
	string str = fpInput->get_text();
	
	// Set up the tokenizer:
	//  * backslash is used for escape chars
	//  * comma, space, and tab are delimiters
	//  * quote groups items together (so delimiters are ignored)
	auto sep = escaped_list_separator<char>("\\", ", \t", "\"");
	tokenizer<escaped_list_separator<char>> tok(str, sep);

	// Copy the message from input to output, and wipe the input
	write(str);
	fpInput->set_text("");

	// Parse the command
	auto token = tok.begin();
	string cmdStr = trim_copy(*token++);

	bool status = false;
	Command* cmd = fpCmdMgr->get(cmdStr);
	// If the command was valid, execute it and store it in the command history
	if (cmd) 
	{
		cmd->setInputStr(str);
		pCmdHist->push(*cmd);
		status = (*cmd)(token, tok.end());
	}
	// Otherwise, store a "dummy" command in the command history
	else pCmdHist->push(BadCommand(str));

	// If the command could not be parsed, display an error message
	if (!status) writeError("---Invalid command---");
}

/*
 * Display a message in the output pane
 */
void VizPrompt::writeInfo(const string& text) { write(text, Info); }
void VizPrompt::writeWarning(const string& text) { write(text, Warning); }
void VizPrompt::writeError(const string& text) { write(text, Error); }
void VizPrompt::write(const string& text, OutputStyle status)
{
	auto buffer = fpOutput->get_buffer();
	auto ins = buffer->end();

	// Apply the appropriate formatting tag
	if      (status == Info)    buffer->insert_with_tag(ins, "\n" + text, "style_info");
	else if (status == Warning) buffer->insert_with_tag(ins, "\n" + text, "style_warning");
	else if (status == Error)   buffer->insert_with_tag(ins, "\n" + text, "style_error");
	else 						buffer->insert(ins, "\n" + text);
}

// Use the up/down arrows to navigate through the history of commands entered
bool VizPrompt::navigate(GdkEventKey* key)
{
	// Handle up/down keypresses, ignore all others
	if (key->keyval == GDK_KEY_Up)
		pCmdHist->step(-1);
	else if (key->keyval == GDK_KEY_Down)
		pCmdHist->step(1);
	else return false;

	// Get the command, set the text of the input bar to the command's text, and place the
	// input bar's cursor at the end of the (newly-added) text
	Command* cmd = pCmdHist->get();
	if (cmd) fpInput->set_text(pCmdHist->get()->getInputStr());
	else fpInput->set_text("");
	fpInput->set_position(-1);
	return true;
}

// Scroll to the bottom of the output panel
void VizPrompt::refreshOutput(Gtk::Allocation& a)
{
	auto adj = fpScrollPane->get_vadjustment();
	adj->set_value(adj->get_upper());
}



