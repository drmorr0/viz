// cmd_parse.cpp: David R. Morrison
// Helper routines for parsing command line arguments

#include "cmd_parse.h"
#include "match.h"

#include <string>

using namespace std;
using namespace boost;

// Match a set of vertices
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, Match& el)
{
	string filterStr = trim_copy(*token++);
	if (token == end) { out->writeError("Truncated match specification"); return false; }
	string operStr = trim_copy(*token++);
	if (token == end) { out->writeError("Truncated match specification"); return false; }
	
	// Don't increment the last time -- the main loop takes care of this
	string valStr = trim_copy(*token);	

	if (!el.setFilter(filterStr))
	{
		out->writeError("Invalid filter specification: " + filterStr);
		return false;
	}
	if (!el.setOperator(operStr))
	{
		out->writeError("Invalid match operator: " + operStr);
		return false;
	}
	if (!el.setValue(valStr))
	{
		out->writeError("Invalid match value: " + valStr);
		return false;
	}
	return true;
}

// Parse a string element
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, string& el)
{
	el = trim_copy(*token);
	return true;
}

// Parse numeric elements
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, double& el)
{
	string valStr = trim_copy(*token);
	try { el = stod(valStr); }
	catch (invalid_argument& e)
	{
		out->writeError("Invalid numeric value: " + valStr);
		return false;
	}
	return true;
}

bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, int& el)
{
	string valStr = trim_copy(*token);
	try { el = stoi(valStr); }
	catch (invalid_argument& e)
	{
		out->writeError("Invalid numeric value: " + valStr);
		return false;
	}
	return true;
}

// Parse a color element
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, Gdk::Color& el)
{
	string colorStr = trim_copy(*token);
	if (!el.set(colorStr))
	{
		out->writeError("Invalid color specification: " + colorStr);
		return false;
	}
	return true;
}

// Still need to implement destructor even though it's virtual
AbstractCmdStructure::~AbstractCmdStructure() { }

// Print out help on the particular command (list each element in order, then provide details for
// each of the different parameters)
string AbstractCmdStructure::help() const
{
	string helpString;
	for (int i = 0; i < helpParts.size(); ++i)
		helpString += "[" + helpParts[i].term + "] ";
	for (int i = 0; i < helpParts.size(); ++i)
		helpString += string("\n    ") + helpParts[i].term + " - " + helpParts[i].desc;
	return helpString;
}
