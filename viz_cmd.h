/*
 * viz_cmd.h: David R. Morrison, March 2014
 *
 * Class to handle the viz command prompt
 */

#include "util.h"

// Sigh... boost...
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::escaped_list_separator<char>>::iterator tok_iter;

class VizCanvas;

class VizCmdPrompt
{
public:
	VizCmdPrompt();

	void parseCommand();
	void displayMessage(const string& text);

private:
	void clear(tok_iter& token);
	void filter(tok_iter& token);
	void format(tok_iter& token);
};
