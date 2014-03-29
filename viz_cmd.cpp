// viz_cmd.cpp: David R. Morrison, March 2014
// Implementation for the viz command prompt

#include "viz_cmd.h"
#include "viz_canvas.h"
#include "more_graph_utils.h"
#include "builder.h"

#include <string>
#include <iostream>

#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

VizCmdPrompt::VizCmdPrompt()
{

	TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->signal_activate().connect(
			sigc::mem_fun(*this, &VizCmdPrompt::parseCommand));

	/* Create some styles for output messages */
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
	output->signal_size_allocate().connect(sigc::mem_fun(*this, &VizCmdPrompt::refreshOutput));
}

void VizCmdPrompt::parseCommand()
{
	string str = TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->get_text();
	auto sep = escaped_list_separator<char>("\\", ", \t", "\"");
	tokenizer<escaped_list_separator<char>> tok(str, sep);

	displayMessage(str);
	bool status = false;

	auto token = tok.begin();
	string cmd = trim_copy(*token++);
	if      (cmd == "filter") status = filter(token, tok.end());
	else if (cmd == "format") status = format(token, tok.end());
	else if (cmd == "clear")  status = clear(token, tok.end());
	else if (cmd == "quit")   Gtk::Main::quit();
	else if (cmd == "exit")   Gtk::Main::quit();

	if (!status) displayMessage("---Invalid command---", Error);

	TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->set_text("");
}

void VizCmdPrompt::displayMessage(const string& text, DisplayStatus status)
{
	auto buffer = TheBuilder::get<Gtk::TextView>("viz_cmd_output")->get_buffer();
	auto ins = buffer->end();

	if      (status == Info)    buffer->insert_with_tag(ins, "\n" + text, "style_info");
	else if (status == Warning) buffer->insert_with_tag(ins, "\n" + text, "style_warning");
	else if (status == Error)   buffer->insert_with_tag(ins, "\n" + text, "style_error");
	else 						buffer->insert(ins, "\n" + text);
}

bool VizCmdPrompt::clear(tok_iter& token, const tok_iter& end)
{
	TheBuilder::getCurrentTab()->showAll();
	return true;
}

bool VizCmdPrompt::filter(tok_iter& token, const tok_iter& end)
{
	VizCanvas* canvas = TheBuilder::getCurrentTab();

	string filterBy = trim_copy(*token++); 
	string matchStr = trim_copy(*token++); 
	string valStr = trim_copy(*token++);
	double value;
	try { value = stod(valStr); } 
	catch (invalid_argument& e) 
	{ 
		displayMessage(string("Invalid numeric value: ") + valStr, Error);
		return false; 
	}

	// The filter command shows only vertices that match the filter.  To accomplish this,
	// we hide everything that does NOT match the filter -- hence the inversion of the 
	// operators here
	MatchOp oper;
	if      (matchStr == "<")  oper = GreaterEq;
	else if (matchStr == "<=") oper = Greater;
	else if (matchStr == "==") oper = NotEq;
	else if (matchStr == "!=") oper = Eq;
	else if (matchStr == ">=") oper = Less;
	else if (matchStr == ">")  oper = LessEq;
	else 
	{
		displayMessage(string("Invalid operator ") + matchStr, Error);
		return false;
	}
	if (token != end) displayMessage(string("Ignoring extra arguments to filter: ") + 
			*token + "...", Warning);

	vector<int> matched = graph::match(*canvas->graph(), filterBy, oper, value);
	if (matched.empty()) displayMessage("No matches found.", Info);
	else canvas->hide(matched);
	return true;
}

bool VizCmdPrompt::format(tok_iter& token, const tok_iter& end)
{
	return true;
}

void VizCmdPrompt::refreshOutput(Gtk::Allocation& a)
{
	auto adj = TheBuilder::get<Gtk::ScrolledWindow>("viz_cmd_scroll")->get_vadjustment();
	adj->set_value(adj->get_upper());
}
