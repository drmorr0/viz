/*
 * David R. Morrison, March 2014
 */

#include "hide.h"
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

const CmdStructure<Match> hideCmd(
		vector<CmdHelp>{{"match", "specify a group of nodes to hide"}}
	);

HideCommand::HideCommand() :
	Command("hide", "Hide nodes in the graph matching specified criteria", 
			{{ /* No subcommands */ }})
{
	/* Do nothing */
}

bool HideCommand::operator()(tok_iter& token, const tok_iter& end)
{
	Match match;
	if (hideCmd.parse(mCmdName, fpOutput, token, end, match))
		TheBuilder::getCurrentTab()->canvas()->hide(match.verts());
	else return false;

	return true;
}





