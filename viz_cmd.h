/*
 * viz_cmd.h: David R. Morrison, March 2014
 *
 * Class to handle the viz command prompt
 */

#include "util.h"

// Sigh... boost...
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::escaped_list_separator<char>>::iterator tok_iter;

#include <gtkmm.h>

class VizCanvas;

class VizCmdPrompt
{
	enum DisplayStatus { Normal, Info, Warning, Error };
public:
	VizCmdPrompt();

	void parseCommand();
	void displayMessage(const string& text, DisplayStatus = Normal);

private:
	bool clear(tok_iter& token, const tok_iter& end);
	bool filter(tok_iter& token, const tok_iter& end);
	bool format(tok_iter& token, const tok_iter& end);

	void refreshOutput(Gtk::Allocation& a);
};
