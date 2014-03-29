/*
 * David R. Morrison, March 2014
 *
 * List of commands supported by the Viz command prompt, along with a help string and a pointer to
 * the function that should be called when the command is entered
 */

#include "viz_cmd.h"
#include "viz_canvas.h"
#include "more_graph_utils.h"
#include "builder.h"

#include <graph.h>

#include <vector>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

const command_template VizCmdPrompt::commands[] = {
	{ "exit", "Quit Viz", &VizCmdPrompt::quit },
	{ "filter", "filter PropertyName Operator Value", &VizCmdPrompt::filter },
	{ "format", "format PropertyName Operator Value FormatSpec", &VizCmdPrompt::format },
	{ "help", "Display help message", &VizCmdPrompt::help },
	{ "showall", "Remove all filtering rules", &VizCmdPrompt::showall },
	{ "quit", "Quit Viz", &VizCmdPrompt::quit },
};

const int VizCmdPrompt::num_commands = 
	sizeof(VizCmdPrompt::commands) / sizeof(VizCmdPrompt::commands[0]);

/***********************************/
/**** command function handlers ****/
/***********************************/

/*
 * Only show nodes matching the specified filter.  The syntax for this command is
 *   filter Property Operator Value
 * where Property is any stored property of the graph, Operator is one of <,<=,==,!=,>=,>, and
 * Value is a numeric constant.
 */
bool VizCmdPrompt::filter(tok_iter& token, const tok_iter& end)
{
    VizCanvas* canvas = TheBuilder::getCurrentTab();

    // Parse the remainder of the command
    double value;
    if (token == end) {displayMessage(string("Too few arguments to filter."), Error); return false;}
    string filterBy = trim_copy(*token++);
    if (token == end) {displayMessage(string("Too few arguments to filter."), Error); return false;}
    string matchStr = trim_copy(*token++);
    if (token == end) {displayMessage(string("Too few arguments to filter."), Error); return false;}
    string valStr = trim_copy(*token++);
    if (token != end) displayMessage(string("Ignoring extra arguments to filter: ") +
            *token + "...", Warning);
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

    // Find the vertices that match and hide them
    vector<int> matched = graph::match(*canvas->graph(), filterBy, oper, value);
    if (matched.empty()) displayMessage("No matches found.", Info);
    else canvas->hide(matched);
    return true;
}

bool VizCmdPrompt::format(tok_iter& token, const tok_iter& end)
{
    return true;
}

/*
 * Display a general help string, or provide more input for a specific command
 */
bool VizCmdPrompt::help(tok_iter& token, const tok_iter& end)
{
	// If we didn't request help on a specific command, display a generic help message
	if (token == end)
	{
		displayMessage(string("The following commands are recognized by Viz") +
					   string(" (type help command for more info):"), Info);
		string cmds = "    ";
		for (int i = 0; i < num_commands; ++i)
			cmds += string(commands[i].command) + " ";
		displayMessage(cmds);
	}
	else
	{
		// Find the command that we requested help for
		int i = 0;
		for ( ; i < num_commands; ++i)
		{
			if (commands[i].command == *token)
			{
				// Display the help message for that command
				displayMessage(commands[i].help, Info);
				break;
			}
		}
		
		// If we didn't find the command in our list, print an error
		if (i == num_commands)
		{
			displayMessage(string("Unknown command ") + *token, Error);
			return false;
		}
	}

	return true;
}

/*
 * Remove all filtering rules
 */
bool VizCmdPrompt::showall(tok_iter& token, const tok_iter& end)
{
    if (token != end) displayMessage(string("Ignoring extra arguments to show: ") + 
            *token + "...", Warning);
    TheBuilder::getCurrentTab()->showAll();
    return true;
}

bool VizCmdPrompt::quit(tok_iter& token, const tok_iter& end)
{
    Gtk::Main::quit();
    return true;
}




