#ifndef MAIN_H
#define MAIN_H
/*
 * main.h: David R. Morrison, Sept. 2013
 *
 * Main header for the graph, tree, and branch-and-bound visualization software
 */

#define BOOL "b"
#define ARG "a"
#define COMMENT "c"
#define BREAK "x"
#define SEP "h"

struct opts
{
	bool opt1;
	bool opt2;
	float opt3;
	int opt4;
};

static const char* optStrings[][3] =
{
	{BOOL, "a", "Option 1"},
	{BOOL, "b", "Option 2"},
	{BREAK, "", ""},
	{ARG, "c", "Option 3"},
	{ARG, "d", "Option 4"},
	{COMMENT, "", "0 - do nothing, 1 - do something, 2 - do everything"},
	{SEP, "", ""},
	{BOOL, "h", "Help"}
};

int main(int argc, char* argv[]);
const char* parseOpts(int argc, char* argv[], opts& options);
void usage(const char* name);

#endif // MAIN_H


