// main.cpp: David R. Morrison, Sept. 2013
// Entry point for the branch-and-bound/graph visualization software

#include "main.h"
#include "viz_io.h"
#include "viz_window.h"

#include <graph.h>
#include <graph_layout.h>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <gtkmm.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <iostream>
using namespace std;
using graph::Graph;

//qstns: scaling for nodes, more than two edges?
int main(int argc, char* argv[])
{
	opts options;
	const char* filename = parseOpts(argc, argv, options);

	Graph testGraph = readJsonTree(filename);
	for (auto i = testGraph.begin(); i != testGraph.end(); ++i)
		testGraph.vertexData(i->first)->radius = 5;
	graph::layoutTreeLevel(testGraph, 0, 0, 50, 25);

	auto app = Gtk::Application::create(argc, argv, "testing.app");
	VizWindow win(testGraph);
	return app->run(win);

	return 0;
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




