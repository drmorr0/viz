// format.cpp: David R. Morrison, March 2014

#include "format.h"
#include "viz_tab.h"
#include "viz_canvas.h"
#include "viz_prompt.h"
#include "more_graph_utils.h"
#include "builder.h"
#include "match.h"

#include <graph.h>

#include <vector>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

// Accepted subcommands for format are: radius, thickness, color, and fill
const CmdStructure<Match, double> radiusCmd({
		{"match", "specify a group of nodes to modify"},
		{"radius", "new radius value for matched nodes"}
	});

const CmdStructure<Match, double> thicknessCmd({
		{"match", "specify a group of nodes to modify"},
		{"thickness", "new border thickness value for matched nodes"}
	});

const CmdStructure<Match, Gdk::Color> colorCmd({
		{"match", "specify a group of nodes to modify"},
		{"color", "new outline color for matched nodes as a Gdk::Color string"}
	});
		 
const CmdStructure<Match, Gdk::Color> fillCmd({
		{"match", "specify a group of nodes to modify"},
		{"color", "new fill color for matched nodes as a Gdk::Color string"}
	});

FormatCommand::FormatCommand() :
	Command("format", "Format nodes in the graph matching specified criteria",
			{{"radius", &radiusCmd},
			 {"thickness", &thicknessCmd},
			 {"color", &colorCmd},
			 {"fill", &fillCmd}})
{
	/* Do nothing */
}

bool FormatCommand::operator()(tok_iter& token, const tok_iter& end)
{
	// First parse the specified subcommand
    if (token == end) { fpOutput->writeError("Too few arguments to " + mCmdName); return false; }
    string subc = trim_copy(*token++);

	if (mSubCommands.count(subc) == 0)
	{
		fpOutput->writeError("Invalid subcommand " + subc);
		return false;
	}

	string subcName = mCmdName + " " + subc;
	Match match(*TheBuilder::getCurrentTab()->graph());

	if (subc == "radius")
	{
		double radius;
		if (radiusCmd.parse(subcName, fpOutput, token, end, match, radius))
			TheBuilder::getCurrentTab()->canvas()->setRadius(match.verts(), radius);
		else return false;
	}
	else if (subc == "thickness")
	{
		double thickness;
		if (thicknessCmd.parse(subcName, fpOutput, token, end, match, thickness))
			TheBuilder::getCurrentTab()->canvas()->setThickness(match.verts(), thickness);
		else return false;
	}
	else if (subc == "color")
	{
		Gdk::Color color;
		if (colorCmd.parse(subcName, fpOutput, token, end, match, color))
			TheBuilder::getCurrentTab()->canvas()->setColor(match.verts(), color);
		else return false;
	}
	else if (subc == "fill")
	{
		Gdk::Color fill;
		if (fillCmd.parse(subcName, fpOutput, token, end, match, fill))
			TheBuilder::getCurrentTab()->canvas()->setFill(match.verts(), fill);
		else return false;
	}

	return true;
}




