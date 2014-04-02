#ifndef VIZ_PROMPT_H
#define VIZ_PROMPT_H
/*
 * viz_prompt.h: David R. Morrison, March 2014
 *
 * These functions handle all of the command prompt and output for Viz.  It uses the Boost tokenizer
 * to parse input commands.  
 *
 */

#include "util.h"

// Sigh... boost...
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::escaped_list_separator<char>>::iterator tok_iter;

#include <gtkmm.h>

namespace VizPrompt
{
	enum DisplayStatus { Normal, Info, Warning, Error };
	void init();

	void parseCommand();
	void displayError(const string& text); 
	void displayMessage(const string& text, DisplayStatus = Normal);
};

#endif // VIZ_PROMPT_H
