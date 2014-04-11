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

FormatCommand::FormatCommand() :
	Command("format PropertyName Operator Value Color [Fill] [Radius] [Thickness]")
{
	/* Do nothing */
}

bool FormatCommand::operator()(tok_iter& token, const tok_iter& end)
{
	// TOKEN: filter string
    if (token == end) { fpOutput->writeError("Too few arguments to format."); return false; }
    string filterBy = trim_copy(*token++);

	// TOKEN: match operator
    if (token == end) { fpOutput->writeError("Too few arguments to format."); return false; }
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
    if (token == end) { fpOutput->writeError("Too few arguments to format."); return false;}
    string valStr = trim_copy(*token++);

	double value;
    try { value = stod(valStr); }
    catch (invalid_argument& e)
    {
		fpOutput->writeError(string("Invalid numeric value: ") + valStr);
        return false;
    }

	// TOKEN: color
    if (token == end) { fpOutput->writeError("Too few arguments to format."); return false; }
	string colorStr = trim_copy(*token++);
	// Make sure the color string is valid TODO
	Gdk::Color color(colorStr);

	// TOKEN: fill color
	Gdk::Color fill("#deaded");
    if (token != end) 
	{ 
		string fillStr = trim_copy(*token++);
		// Make sure the color string is valid TODO
		fill = Gdk::Color(fillStr);
	}

	// TOKEN: radius (optional)
	string radStr; if (token != end) radStr = trim_copy(*token++);

	double radius = -1;
	if (!radStr.empty())
	{
		try { radius = stod(radStr); if (radius < 0) throw invalid_argument("negative"); }
		catch (invalid_argument& e)
		{
			fpOutput->writeError(string("Invalid numeric radius: ") + radStr);
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
			fpOutput->writeError(string("Invalid numeric thickness: ") + thickStr);
			return false;
		}
	}

	// Ignore additional arguments to format
    if (token != end) 
		fpOutput->writeWarning(string("Ignoring extra arguments to format: ") + *token + "...");

    return (*this)(filterBy, oper, value, color, fill, radius, thickness);
}

bool FormatCommand::operator()(const string& filterBy, MatchOp oper, double value, 
		const Gdk::Color& color, const Gdk::Color& fill, double radius, double thickness)
{
	VizCanvas* canvas = TheBuilder::getCurrentTab();

    vector<int> matched = graph::match(*canvas->graph(), filterBy, oper, value);
    if (matched.empty()) fpOutput->writeInfo("No matches found.");
    else canvas->format(matched, color, fill, radius, thickness);

	return true;
}



