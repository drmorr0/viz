// cmd_parse.cpp: David R. Morrison
// Helper routines for parsing command line arguments

#include "cmd_parse.h"
#include "match.h"

#include <string>

using namespace std;

bool parseElement(tok_iter& token, const tok_iter& end, Match& el)
{
	return true;
}

bool parseElement(tok_iter& token, const tok_iter& end, string& el)
{
	el = "asdf";
	return true;
}

bool parseElement(tok_iter& token, const tok_iter& end, double& el)
{
	el = 1.0;
	return true;
}

bool parseElement(tok_iter& token, const tok_iter& end, int& el)
{
	el = 1;
	return true;
}

bool parseElement(tok_iter& token, const tok_iter& end, Gdk::Color& el)
{
	el = Gdk::Color("#ffffff");
	return true;
}

AbstractCmdStructure::~AbstractCmdStructure() { }

string AbstractCmdStructure::help() const
{
	string helpString;
	for (int i = 0; i < helpParts.size(); ++i)
		helpString += "[" + helpParts[i].term + "] ";
	for (int i = 0; i < helpParts.size(); ++i)
		helpString += string("\n    ") + helpParts[i].term + " - " + helpParts[i].desc;
	return helpString;
}
