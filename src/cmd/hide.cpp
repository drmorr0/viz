/*
 * David R. Morrison, March 2014
 */

#include "hide.h"
#include "viz_tab.h"
#include "viz_canvas.h"
#include "viz_prompt.h"
#include "more_graph_utils.h"
#include "builder.h"

#include <graph.h>

#include <vector>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

HideCommand::HideCommand() :
	Command("hide PropertyName Operator Value")
{
	/* Do nothing */
}

bool HideCommand::operator()(tok_iter& token, const tok_iter& end)
{
	// TOKEN: filter string
    if (token == end) { fpOutput->writeError("Too few arguments to filter."); return false; }
    string filterBy = trim_copy(*token++);

	// TOKEN: match operator
    if (token == end) { fpOutput->writeError("Too few arguments to filter."); return false; }
    string matchStr = trim_copy(*token++);
    MatchOp oper;
    if      (matchStr == "<")  oper = Less;
    else if (matchStr == "<=") oper = LessEq;
    else if (matchStr == "==") oper = Eq;
    else if (matchStr == "!=") oper = NotEq;
    else if (matchStr == ">=") oper = GreaterEq;
    else if (matchStr == ">")  oper = Greater;
    else { fpOutput->writeError(string("Invalid operator ") + matchStr); return false; }

	// TOKEN: value
    if (token == end) { fpOutput->writeError("Too few arguments to filter."); return false; }
    string valStr = trim_copy(*token++);

    double value;
    try { value = stod(valStr); }
    catch (invalid_argument& e)
    {
		fpOutput->writeError(string("Invalid numeric value: ") + valStr);
        return false;
    }

	// Extra tokens are ignored
    if (token != end) 
		fpOutput->writeWarning(string("Ignoring extra arguments to filter: ") + *token + "...");

	// Do the filter
	return (*this)(filterBy, oper, value);

}

bool HideCommand::operator()(const string& filterBy, MatchOp oper, double value)
{
    VizTab* tab = TheBuilder::getCurrentTab();

    // Find the vertices that match and hide them
    vector<int> matched = graph::match(*tab->graph(), filterBy, oper, value);
    if (matched.empty()) fpOutput->writeInfo("No matches found.");
    else tab->canvas()->hide(matched);
    return true;
}




