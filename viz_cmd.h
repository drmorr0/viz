/*
 * viz_cmd.h: David R. Morrison, March 2014
 *
 * This class handles all of the command prompt and output for Viz.  It uses the Boost tokenizer to
 * parse input commands.  The important thing to note is that input commands are sequentially
 * applied.  So, for example, filtering all of the nodes with lower bound <= 5 and then filtering 
 * all * of the nodes with lower bound > 3 will only show the nodes with lower bound in (3, 5].  The
 * same is true for node formatting.  This is desired behavior and future updates should make sure
 * they operate in the same way
 *
 * Eventually, we want to be able to read a script from a file, so that we can quickly and easily
 * format lots of different graphs the same way TODO
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
	bool show(tok_iter& token, const tok_iter& end);
	bool filter(tok_iter& token, const tok_iter& end);
	bool format(tok_iter& token, const tok_iter& end);

	void refreshOutput(Gtk::Allocation& a);
};
