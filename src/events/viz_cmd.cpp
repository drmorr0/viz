// viz_cmd.cpp: David R. Morrison, March 2014
// Implementation for the viz command prompt

#include "viz_cmd.h"
#include "builder.h"

#include <string>
#include <iostream>

#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

VizCmdPrompt::VizCmdPrompt()
{

	// Set up an event handler when the command prompt has focus and the user presses enter
	TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->signal_activate().connect(
			sigc::mem_fun(*this, &VizCmdPrompt::parseCommand));

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
	output->signal_size_allocate().connect(sigc::mem_fun(*this, &VizCmdPrompt::refreshOutput));
}

/*
 * Parse a command that is typed into the command prompt
 *
 * Currently, we handle each command separately; however, we'll want to eventually create
 * documentation for the various commands, and make it easier to extend so this will probably need
 * to be rethought (TODO)
 *
 * Supported commands:
 *
 * filter - only show items matching the given condition
 * format - stylize items matching the given condition (TODO)
 * showall - show all nodes
 * clear - clear all formatting (TODO)
 * quit, exit - terminate the program
 * help - display a short help message (TODO)
 */
void VizCmdPrompt::parseCommand()
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
	bool status = false;
	auto token = tok.begin();
	string cmd = trim_copy(*token++);

	for (int i = 0; i < num_commands; ++i)
	{
		if (cmd == commands[i].command)
		{
			status = (this->*commands[i].exec)(token, tok.end());
			break;
		}
	}

	// If the command could not be parsed, display an error message
	if (!status) displayMessage("---Invalid command---", Error);

}

/*
 * Display a message in the output pane
 */
void VizCmdPrompt::displayMessage(const string& text, DisplayStatus status)
{
	auto buffer = TheBuilder::get<Gtk::TextView>("viz_cmd_output")->get_buffer();
	auto ins = buffer->end();

	// Apply the appropriate formatting tag
	if      (status == Info)    buffer->insert_with_tag(ins, "\n" + text, "style_info");
	else if (status == Warning) buffer->insert_with_tag(ins, "\n" + text, "style_warning");
	else if (status == Error)   buffer->insert_with_tag(ins, "\n" + text, "style_error");
	else 						buffer->insert(ins, "\n" + text);
}

// Scroll to the bottom of the output panel
void VizCmdPrompt::refreshOutput(Gtk::Allocation& a)
{
	auto adj = TheBuilder::get<Gtk::ScrolledWindow>("viz_cmd_scroll")->get_vadjustment();
	adj->set_value(adj->get_upper());
}
