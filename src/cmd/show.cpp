/*
 * David R. Morrison, March 2014
 *
 * List of commands supported by the Viz command prompt, along with a help string and a pointer to
 * the function that should be called when the command is entered
 */

#include "show.h"
#include "viz_canvas.h"
#include "viz_prompt.h"
#include "builder.h"

#include <graph.h>

#include <string>
using namespace std;

/*
 * Remove all filtering rules
 */
ShowCommand::ShowCommand() :
	Command("Remove all filtering rules")
{
	/* Do nothing */
}

bool ShowCommand::operator()(tok_iter& token, const tok_iter& end)
{
    if (token != end) VizPrompt::displayMessage(string("Ignoring extra arguments to show: ") + 
            *token + "...", VizPrompt::Warning);
    TheBuilder::getCurrentTab()->showAll();
    return true;
}

