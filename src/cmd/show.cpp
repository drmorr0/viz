/*
 * David R. Morrison, March 2014
 */

#include "show.h"
#include "viz_tab.h"
#include "viz_canvas.h"
#include "viz_prompt.h"
#include "builder.h"

#include <graph.h>

#include <string>
using namespace std;

ShowCommand::ShowCommand() :
	Command("show", "Remove all filtering rules", {{ /* No subcommands */ }})
{
	/* Do nothing */
}

bool ShowCommand::operator()(tok_iter& token, const tok_iter& end)
{
    if (token != end) 
		fpOutput->writeWarning(string("Ignoring extra arguments to show: ") + *token + "...");
    TheBuilder::getCurrentTab()->canvas()->showAll();
    return true;
}

