/*
 * David R. Morrison, March 2014
 */

#include "hide.h"
#include "viz_canvas.h"
#include "viz_prompt.h"
#include "more_graph_utils.h"
#include "builder.h"

#include <graph.h>

#include <vector>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

/*
 * Only show nodes matching the specified filter.  The syntax for this command is
 *   filter Property Operator Value
 * where Property is any stored property of the graph, Operator is one of <,<=,==,!=,>=,>, and
 * Value is a numeric constant.
 */
HideCommand::HideCommand() :
	Command("hide PropertyName Operator Value")
{
	/* Do nothing */
}

bool HideCommand::operator()(tok_iter& token, const tok_iter& end)
{
	// TOKEN: filter string
    if (token == end) { VizPrompt::displayError("Too few arguments to filter."); return false; }
    string filterBy = trim_copy(*token++);

	// TOKEN: match operator
    if (token == end) { VizPrompt::displayError("Too few arguments to filter."); return false; }
    string matchStr = trim_copy(*token++);
    MatchOp oper;
    if      (matchStr == "<")  oper = Less;
    else if (matchStr == "<=") oper = LessEq;
    else if (matchStr == "==") oper = Eq;
    else if (matchStr == "!=") oper = NotEq;
    else if (matchStr == ">=") oper = GreaterEq;
    else if (matchStr == ">")  oper = Greater;
    else { VizPrompt::displayError(string("Invalid operator ") + matchStr); return false; }

	// TOKEN: value
    if (token == end) { VizPrompt::displayError("Too few arguments to filter."); return false; }
    string valStr = trim_copy(*token++);

    double value;
    try { value = stod(valStr); }
    catch (invalid_argument& e)
    {
		VizPrompt::displayError(string("Invalid numeric value: ") + valStr);
        return false;
    }

	// Extra tokens are ignored
    if (token != end) 
	{
		VizPrompt::displayMessage(string("Ignoring extra arguments to filter: ") +
            *token + "...", VizPrompt::Warning);
	}

	// Do the filter
	return (*this)(filterBy, oper, value);

}

bool HideCommand::operator()(const string& filterBy, MatchOp oper, double value)
{
    VizCanvas* canvas = TheBuilder::getCurrentTab();

    // Find the vertices that match and hide them
    vector<int> matched = graph::match(*canvas->graph(), filterBy, oper, value);
    if (matched.empty()) VizPrompt::displayMessage("No matches found.", VizPrompt::Info);
    else canvas->hide(matched);
    return true;
}




