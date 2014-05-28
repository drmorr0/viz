#ifndef SAVE_H
#define SAVE_H
/*
 * David R. Morrison, April 2014
 *
 * This command saves a graph (with formatting applied) to an image file
 * 	save Filename
 * where Filename is the image file to save.  The image file format is auto-detected from the file's
 * extension, and defaults to PNG if it can't figure out what format you want.  
 *   Currently supported formats: any listed in Gdk::Pixbuf::get_formats(), EPS (TODO)
 */

#include "cmd.h"
#include "cmd_parse.h"

class SaveCommand : public Command
{
public:
	SaveCommand();
	Command* clone() const { return new SaveCommand(*this); }
	bool operator()(tok_iter& token, const tok_iter& end);

	// Subcommands
	bool saveGraph(const std::string& filename, const std::string& format, double width);

private:
	bool isSupportedFormat(std::string& format);
};

#endif // SAVE_H
