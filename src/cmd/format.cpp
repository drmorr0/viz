/*
 * David R. Morrison, March 2014
 */

#include "format.h"
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
 * Stylize nodes matching the filter according to the given style specification
 *   format Property Operator Value Color [Radius] [Thickness]
 * where Property, Operator, and Value are the same as for filter.  Color is an  HTML color
 * specification (#ff0000), and Radius and Thickness are optional numeric arguments that specify
 * the radius and thickness of the matched vertices.
 */
FormatCommand::FormatCommand() :
	Command("format PropertyName Operator Value Color [Radius] [Thickness]")
{
	/* Do nothing */
}

bool FormatCommand::operator()(tok_iter& token, const tok_iter& end)
{
	// TOKEN: filter string
    if (token == end) { VizPrompt::displayError("Too few arguments to format."); return false; }
    string filterBy = trim_copy(*token++);

	// TOKEN: match operator
    if (token == end) { VizPrompt::displayError("Too few arguments to format."); return false; }
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
    if (token == end) { VizPrompt::displayError("Too few arguments to format."); return false;}
    string valStr = trim_copy(*token++);

	double value;
    try { value = stod(valStr); }
    catch (invalid_argument& e)
    {
		VizPrompt::displayError(string("Invalid numeric value: ") + valStr);
        return false;
    }

	// TOKEN: color
    if (token == end) { VizPrompt::displayError("Too few arguments to format."); return false; }
	string colorStr = trim_copy(*token++);
	// Make sure the color string is valid TODO
	Gdk::Color color(colorStr);

	// TOKEN: radius (optional)
	string radStr; if (token != end) radStr = trim_copy(*token++);

	double radius = -1;
	if (!radStr.empty())
	{
		try { radius = stod(radStr); if (radius < 0) throw invalid_argument("negative"); }
		catch (invalid_argument& e)
		{
			VizPrompt::displayError(string("Invalid numeric radius: ") + radStr);
			return false;
		}
	}

	// TOKEN: thickness (optional)
	string thickStr; if (token != end) thickStr = trim_copy(*token++);

	double thickness = -1;
	if (!thickStr.empty())
	{
		try { thickness = stod(thickStr); if (thickness < 0) throw invalid_argument("negative"); }
		catch (invalid_argument& e)
		{
			VizPrompt::displayError(string("Invalid numeric thickness: ") + thickStr);
			return false;
		}
	}

	// Ignore additional arguments to format
    if (token != end) 
	{
		VizPrompt::displayMessage(string("Ignoring extra arguments to format: ") +
            *token + "...", VizPrompt::Warning);
	}

    return (*this)(filterBy, oper, value, color, radius, thickness);
}

bool FormatCommand::operator()(const string& filterBy, MatchOp oper, double value, 
		const Gdk::Color color, double radius, double thickness)
{
	VizCanvas* canvas = TheBuilder::getCurrentTab();

    vector<int> matched = graph::match(*canvas->graph(), filterBy, oper, value);
    if (matched.empty()) VizPrompt::displayMessage("No matches found.", VizPrompt::Info);
    else canvas->format(matched, color, radius, thickness);

	return true;
}



