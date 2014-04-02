// viz_prompt.cpp: David R. Morrison, March 2014
// Implementation functions for the viz command prompt

#include "viz_prompt.h"
#include "builder.h"

#include "cmd.h"
#include "format.h"
#include "help.h"
#include "hide.h"
#include "load.h"
#include "show.h"
#include "quit.h"

#include <string>
#include <iostream>

#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

namespace VizPrompt
{

// Scroll to the bottom of the output panel
void refreshOutput(Gtk::Allocation& a)
{
	auto adj = TheBuilder::get<Gtk::ScrolledWindow>("viz_cmd_scroll")->get_vadjustment();
	adj->set_value(adj->get_upper());
}

void init()
{
	// Set up an event handler when the command prompt has focus and the user presses enter
	TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->signal_activate().connect(&parseCommand);

	// Create some formatting styles -- we may want to create these elsewhere if 
	// we plan to use them for multiple buffers (TODO)
	auto output = TheBuilder::get<Gtk::TextView>("viz_cmd_output");
	auto errorTag = output->get_buffer()->create_tag("style_error");
	errorTag->property_weight() = Pango::WEIGHT_BOLD;
	errorTag->property_foreground() = "#ff0000";
	auto infoTag = output->get_buffer()->create_tag("style_info");
	infoTag->property_weight() = Pango::WEIGHT_BOLD;
	infoTag->property_foreground() = "#0000ff";
	auto warnTag = output->get_buffer()->create_tag("style_warning");
	warnTag->property_weight() = Pango::WEIGHT_BOLD;
	warnTag->property_foreground() = "#ffa500";

	// When the output window size is allocated, then scroll to the bottom of the output window
	// Necessary because the new size might not be computed until a redraw
	output->signal_size_allocate().connect(&refreshOutput);

	// Register commands accepted by the prompt
	CommandManager::registerCommand("exit", QuitCommand());
	CommandManager::registerCommand("format", FormatCommand());
	CommandManager::registerCommand("help", HelpCommand());
	CommandManager::registerCommand("hide", HideCommand());
	CommandManager::registerCommand("load", LoadCommand());
	CommandManager::registerCommand("show", ShowCommand());
	CommandManager::registerCommand("quit", QuitCommand());
}

/*
 * Parse a command that is typed into the command prompt
 */
void parseCommand()
{
	string str = TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->get_text();
	
	// Set up the tokenizer:
	//  * backslash is used for escape chars
	//  * comma, space, and tab are delimiters
	//  * quote groups items together (so delimiters are ignored)
	auto sep = escaped_list_separator<char>("\\", ", \t", "\"");
	tokenizer<escaped_list_separator<char>> tok(str, sep);

	// Copy the message from input to output, and wipe the input
	displayMessage(str);
	TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->set_text("");

	// Parse the command
	auto token = tok.begin();
	string cmdStr = trim_copy(*token++);

	bool status = false;
	Command* cmd = CommandManager::get(cmdStr);
	if (cmd) status = (*cmd)(token, tok.end());

	// If the command could not be parsed, display an error message
	if (!status) displayMessage("---Invalid command---", Error);
}

/*
 * Display a message in the output pane
 */
void displayError(const string& text) { displayMessage(text, Error); }
void displayMessage(const string& text, DisplayStatus status)
{
	auto buffer = TheBuilder::get<Gtk::TextView>("viz_cmd_output")->get_buffer();
	auto ins = buffer->end();

	// Apply the appropriate formatting tag
	if      (status == Info)    buffer->insert_with_tag(ins, "\n" + text, "style_info");
	else if (status == Warning) buffer->insert_with_tag(ins, "\n" + text, "style_warning");
	else if (status == Error)   buffer->insert_with_tag(ins, "\n" + text, "style_error");
	else 						buffer->insert(ins, "\n" + text);
}

}; // namespace VizPrompt
