// main.cpp: David R. Morrison, Sept. 2013
// Entry point for the branch-and-bound/graph visualization software

#include "main.h"
#include "graph.h"
#include "graph_io.h"
#include "graph_layout.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <gtkmm.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <iostream>
using namespace std;

class GraphCanvas : public Gtk::DrawingArea
{
public:
	GraphCanvas()
	{
#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
		signal_draw().connect(sigc::mem_fun(*this, &GraphCanvas::onDraw), false);
#endif // GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
	}

private:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
	{
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		int numNodes = 0; 
		int r = height / 12.0; 
		int x = width / 2.0; 	
		int y = height / 12.0; 
		cr->arc(x, y, r, 0, 2.0 * M_PI);		
		cr->stroke(); 
		cr->move_to(x, y+r); 
		cr->line_to((x/2.0), y+2*r); 
		cr->stroke();
		cr->move_to(x, y+r); 
		cr->line_to((3.0*x) / 2.0, y+2*r);
		cr->stroke();
		// to draw left node and its edges 
		for (int i = 0 ; i <= 2; i ++)
		{		 
			x = x/2; 
			y = y+3*r; 
			cr->arc(x,y,r,0,2.0* M_PI);  
			cr->stroke();
			cr->move_to(x, y+r); 
			cr->line_to(x/2, y+2*r); 
			cr->stroke(); 
			cr->move_to(x, y+r); 
			cr->line_to((3.0*x) / 2.0, y+2*r); 
			cr->stroke();
	    	}	 
		
		x = width / 2.0; 
		y = (height/12.0);
		  
		// to draw right node and its edges 
		for (int i = 0; i < 2; i ++)
		{	
			x = (x+width)/2;
			y = y+3*r;  	
			cr->arc(x,y,r,0,2.0* M_PI);
                	cr->stroke();
			cr->move_to(x, y+r);
                	cr->line_to((x+(width/2))/2, y+2*r);
                	cr->stroke();
                	cr->move_to(x, y+r);
                	cr->line_to((x+(width))/2, y+2*r);
                	cr->stroke();
			cr->move_to(x,y+r);  
			cr->stroke();
			
			//x = (x+width)/2; 
			//y = y + 3*r; 
			//cr->arc(x,y,r,0,2.0*M_PI);
			//cr->stroke();    
		}
			 
                /*
		cr->save();
		cr->set_source_rgb(0.86, 0.85, 0.47);
		cr->paint();
		cr->restore();

		cr->save();
		cr->set_line_width(20.0);
		cr->rectangle(0.0, 0.0, width, height);
		cr->stroke();

		cr->set_source_rgba(0, 0, 0, 0.7);
		cr->arc(width / 2.0, height / 2.0, height / 4.0, 0, 2.0 * M_PI);
		cr->stroke();

		cr->move_to(width / 4.0, height / 4.0);
		cr->line_to(width * 3.0 / 4.0, height * 3.0 / 4.0);
		cr->stroke();
		cr->restore();
		*/
	}
};
//qstns: scaling for nodes, more than two edges?
int main(int argc, char* argv[])
{
	opts options;
	const char* filename = parseOpts(argc, argv, options);

	drm::Graph testGraph = drm::readJsonTree(filename);
	for (auto i = testGraph.begin(); i != testGraph.end(); ++i)
		testGraph.vertexData(i->first)->radius = 5;
	drm::GraphUtils::layoutTreeLevel(testGraph, {0, 0}, 50, 25);

	for (auto i = testGraph.begin(); i != testGraph.end(); ++i)
	{
		auto data = testGraph.vertexData(i->first);
		printf("Vertex %d: radius %d, center (%d, %d)\n", i->first, data->radius,
				data->center.x, data->center.y);
	}
	exit(0);

	auto app = Gtk::Application::create(argc, argv, "testing");
	Gtk::Window window;
	window.set_default_size(10000, 10000);
	window.set_position(Gtk::WIN_POS_CENTER);
	window.maximize(); 
	//GtkWidget * image_viewer = gtk_image_view_new();  
	GraphCanvas canvas;
	canvas.set_size_request(600,400);

	window.add(canvas);
	canvas.show();

	return app->run(window);
}

const char* parseOpts(int argc, char* argv[], opts& options)
{
	options.opt1 = false;
	options.opt2 = false;
	options.opt3 = 4.3;
	options.opt4 = 10;

	int len = sizeof(optStrings)/(3 * sizeof(char*));
	int numOpts = 0;
	for (int i = 0; i < len; ++i)
		if (optStrings[i][0][0] == BOOL[0] || optStrings[i][0][0] == ARG[0]) ++numOpts;

	char flags[numOpts * 2 + 1];
	int pos = 0;
	for (int i = 0; i < len; ++i)
	{
		if (optStrings[i][0][0] != BOOL[0] && optStrings[i][0][0] != ARG[0]) continue;
		flags[pos++] = optStrings[i][1][0];
		if (optStrings[i][0][0] == ARG[0]) flags[pos++] = ':';
	}
	flags[pos] = '\0';

	int opt;
	while ((opt = getopt(argc, argv, flags)) != -1)
	{
		switch (opt)
		{
		case 'a':
			options.opt1 = true;
			break;
		case 'b':
			options.opt2 = true;
			break;
		case 'c':
			options.opt3 = atof(optarg);
			break;
		case 'd':
			options.opt4 = atoi(optarg);
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
		default:
			usage(argv[0]);
			exit(-1);
		}
	}

	if (optind == argc - 1)
		return argv[optind];
	else
	{
		fprintf(stderr, "No file specified\n");
		usage(argv[0]);
		exit(-1);
	}
}

void usage(const char* name)
{
	int len = sizeof(optStrings)/(3 * sizeof(char*));
	printf("%s <options> [filename]\n", name);
	for (int i = 0; i < len; ++i)
	{
		if (optStrings[i][0][0] == BREAK[0]) 
			printf("\n");
		else if (optStrings[i][0][0] == SEP[0])
			printf("---------------------------------------------\n");
		else if (optStrings[i][0][0] == COMMENT[0])
			printf("\t\t%s\n", optStrings[i][2]);
		else
			printf("\t-%s: %s\n", optStrings[i][1], optStrings[i][2]);
	}
	printf("\n");
}




