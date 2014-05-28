#ifndef CMD_PARSE_H
#define CMD_PARSE_H
/*
 * cmd_parse.h: David R. Morrison, May 2014
 *
 * Ok, so this gets a little bit complicated.  The main goal is the following: when I add new types
 * of commands to Viz!, I would like a compile-time check to make sure I've specified what types of
 * parameters the command is expecting, and that I've provided the appropriate documentation for
 * each of those parameters.  I would also like to not have to duplicate the parsing code for each
 * different command.
 *
 * To accomplish all of these goals, I use variadic templates to create a command structure object.
 * The template parameters indicate the type of each parameter in the command.  Then I provide
 * appropriate parsing routines for each of the different types, but let the compiler figure out
 * which ones to call.
 *
 * To handle documentation, the variadic template object inherits from an AbstractCmdStructure,
 * which stores and handles all of the help information needed for the command.
 *
 */

#include "cmd.h"
#include "types.h"

#include <boost/algorithm/string/trim.hpp>

class Match;

// Parse individual elements of a command

bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, Match& el);
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, std::string& el);
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, double& el);
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, int& el);
bool parseElement(tok_iter& token, const tok_iter& end, Outputter* out, Gdk::Color& el);

// This struct stores documentation information for each of the command parameters
struct CmdHelp
{
	std::string term;
	std::string desc;
};

// The abstract base class for CmdStructures
struct AbstractCmdStructure
{
	AbstractCmdStructure(const std::vector<CmdHelp>& h) : helpParts(h) { } 
	virtual ~AbstractCmdStructure() = 0;  // Can't create an AbstractCmdStructure
	const std::vector<CmdHelp> helpParts;
	std::string help() const; 
};

// The templated subclasses for different command structures
template <typename T, typename... Rest>
struct CmdStructure : public AbstractCmdStructure
{
	// Run-time check to make sure that the documentation provided for the command matches the
	// specified parameters for the command; this is a little bit of a hack, but it's not going to
	// provide any overhead since it will only get called once when the command structure is
	// instantiated, and normal users should never see it fail
	CmdStructure(const std::vector<CmdHelp>& h) : AbstractCmdStructure(h) 
		{ assert(helpParts.size() == sizeof...(Rest) + 1); }
	~CmdStructure() { /* do nothing */ }

	// Recursive routine to parse the command
	bool parse(std::string name, Outputter* out, tok_iter& token, const tok_iter& end,
			T& el, Rest&... rest) const { return parseHelper(name, out, token, end, el, rest...); }
};

// Base case for the recursive parsing routing: always return true, but print a warning in case the
// user specified extra parameters that will get ignored.  Need to mark this as inline to avoid
// multiple-definition problems
inline bool parseHelper(std::string cmdName, Outputter* out, tok_iter& token, const tok_iter& end) 
{
	if (token != end) 
		out->writeWarning("Ignoring extra arguments to " + cmdName + ": " + 
				boost::trim_copy(*token));
	return true;
}

// Recursive command-parsing call (this can't be a part of the CmdStructure class because we're
// going to be stripping off one element from the template list each time, and thus the argument
// list won't match up with the CmdStructure object template.
template <typename T, typename... Rest>
bool parseHelper(std::string name, Outputter* out, tok_iter& token, 
		const tok_iter& end, T& el, Rest&... rest) 
{
	// Abort with an error if the user didn't provide all the proper parameters
	if (token == end)
		{ out->writeError("Too few arguments to " + name); return false; }

	// Try to parse the next element in the command; on success, recurse
	else if (!parseElement(token, end, out, el)) return false;
	else return parseHelper(name, out, ++token, end, rest...);
}


#endif // CMD_PARSE_H
