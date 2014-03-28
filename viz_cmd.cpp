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
}

void VizCmdPrompt::parseCommand()
{
	string str = TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->get_text();
	auto sep = escaped_list_separator<char>("\\", ", \t", "\"");
	tokenizer<escaped_list_separator<char>> tok(str, sep);

	displayMessage(str);

	auto token = tok.begin();
	string cmd = trim_copy(*token);
	if      (cmd == "filter") filter(++token);
	else if (cmd == "format") format(++token);
	else if (cmd == "clear")  clear(++token);
	else displayMessage("---Invalid command---");

	TheBuilder::get<Gtk::Entry>("viz_cmd_prompt")->set_text("");
}

void VizCmdPrompt::displayMessage(const string& text)
{
	Gtk::TextView* output = TheBuilder::get<Gtk::TextView>("viz_cmd_output");
	output->get_buffer()->insert_at_cursor("\n" + text);
}

void VizCmdPrompt::clear(tok_iter& token)
{
	TheBuilder::getCurrentTab()->showAll();
}

void VizCmdPrompt::filter(tok_iter& token)
{
	VizCanvas* canvas = TheBuilder::getCurrentTab();

	string filterBy = trim_copy(*token); ++token;
	string matchStr = trim_copy(*token); ++token;
	double value = stod(trim_copy(*token));

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

	vector<int> matched = graph::match(*canvas->graph(), filterBy, oper, value);
	canvas->hide(matched);
}

void VizCmdPrompt::format(tok_iter& token)
{
}

