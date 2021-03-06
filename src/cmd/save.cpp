/*
 * David R. Morrison, March 2014
 */

#include "save.h"
#include "viz_tab.h"
#include "viz_canvas.h"
#include "viz_window.h"
#include "viz_prompt.h"
#include "viz_io.h"
#include "builder.h"

#include <graph.h>

#include <fstream>
#include <string>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>  

using namespace std;
using namespace boost;
using Cairo::Surface;
using Cairo::PsSurface;
using Cairo::PdfSurface;
using Cairo::SvgSurface;
using Cairo::ImageSurface;

const CmdStructure<string, int> saveCmd({
		{"filename", "file to save to (type is determined by extension)"},
		{"width", "width of saved file in pixels"}
	});

SaveCommand::SaveCommand() :
	Command("save", "Save the graph along with its formatting to a file", 
			{{ /* No subcommands TODO? */}})
{
	/* Do nothing */
}

bool SaveCommand::operator()(tok_iter& token, const tok_iter& end)
{
	string filename; int width;
	saveCmd.parse(mCmdName, fpOutput, token, end, filename, width);
	
	// If no format specified, try to guess the format from the file extension
	// According to StackOverflow, this is not a scalable solution, but if it doesn't work
	// that's ok because we have a fallback TODO
	string format("ps");
	size_t pos = filename.rfind('.');
	if (pos != string::npos)
	{
		string test = filename.substr(pos + 1);
		if (!isSupportedFormat(test))
			fpOutput->writeWarning("Could not identify image format: default to PostScript");
		else format = test;
	}

	return saveGraph(filename, format, width);
}

bool SaveCommand::saveGraph(const string& filename, const string& format, double width)
{
	VizTab* tab = TheBuilder::getCurrentTab();
	BoundingBox canvBnd = tab->canvas()->bounds();
	double scale = width == -1 ? 1 :  width / canvBnd.width();
	double outputW = scale * canvBnd.width();
	double outputH = scale * canvBnd.height();

	// Cairo uses different kinds of surfaces to render to
	Cairo::RefPtr<Surface> surface;
	if (format == "ps" || format == "eps")
	{
		// Have to hack this a little bit; a PsSurface is a type of surface, so the inheritance
		// works, but I can't call any PsSurface-specific functions.  So I have to create a
		// PsSurface, call the functions that I want, and then convert it to a Surface
		Cairo::RefPtr<PsSurface> pssurface = 
			PsSurface::create(filename, outputW, outputH);
		if (format == "eps")
			pssurface->set_eps(true);
		surface = pssurface;
	}
	else if (format == "pdf")
		surface = PdfSurface::create(filename, outputW, outputH);
	else if (format == "svg")
		surface = SvgSurface::create(filename, outputW, outputH);
	else 
		surface = ImageSurface::create(Cairo::FORMAT_ARGB32, outputW, outputH);

	// If we aren't saving to png or gif, then we fill in a white background; otherwise, 
	// we will get a transparent background (TODO may want to let user specify background 
	// color somehow?)
	CairoContext destContext = Cairo::Context::create(surface);
	if (format != "png" && format != "gif")
	{
		destContext->save();
		destContext->set_source_rgb(1,1,1);
		destContext->paint();
		destContext->restore();
	}
	
	// Draw the image to the surface
	tab->canvas()->render(destContext, scale);

	// If we aren't using Cairo to render, create a Gdk::Pixbuf and save to the specified file
	if (format != "ps" && format != "pdf" && format != "eps" && format != "svg")
	{
		auto pixbuf = Gdk::Pixbuf::create(surface, 0, 0, outputW, outputH);
		pixbuf->save(filename, format);
	}

	return true;
}

bool SaveCommand::isSupportedFormat(string& format)
{
	// GTK spells out "jpeg", so we need to correct it if the user enters jpg instead
	if (format == "jpg") format = "jpeg";

	// Check the supported formats in GTK
	auto formatList = Gdk::Pixbuf::get_formats();
	for (int i = 0; i < formatList.size(); ++i)
		if (format == formatList[i].get_name()) 
			return true;
	
	// Additional formats supported by Cairo
	if (format == "ps" || format == "eps" || format == "pdf" || format == "svg")
		return true;

	// Format not found
	return false;
}


