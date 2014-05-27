#ifndef CMD_PARSE_H
#define CMD_PARSE_H
/*
 * cmd_parse.h: David R. Morrison, May 2014
 *
 * Template instantiation for command parsing
 */

#include "cmd.h"
#include "types.h"
#include "outputter.h"

class Match;

bool parseElement(tok_iter& token, const tok_iter& end, Match& el);
bool parseElement(tok_iter& token, const tok_iter& end, std::string& el);
bool parseElement(tok_iter& token, const tok_iter& end, double& el);
bool parseElement(tok_iter& token, const tok_iter& end, int& el);
bool parseElement(tok_iter& token, const tok_iter& end, Gdk::Color& el);

class Outputter;

struct CmdHelp
{
	std::string term;
	std::string desc;
};

struct AbstractCmdStructure
{
	AbstractCmdStructure(const std::vector<CmdHelp>& h) : helpParts(h) { } 
	virtual ~AbstractCmdStructure() = 0;  // Can't create an AbstractCmdStructure
	const std::vector<CmdHelp> helpParts;
	std::string help() const; 
};

template <typename T, typename... Rest>
struct CmdStructure : public AbstractCmdStructure
{
	CmdStructure(const std::vector<CmdHelp>& h) : AbstractCmdStructure(h) 
		{ assert(helpParts.size() == sizeof...(Rest) + 1); }
	~CmdStructure() { }
	bool parse(std::string name, Outputter* out, tok_iter& token, const tok_iter& end,
			T& el, Rest&... rest) const { return parseHelper(name, out, token, end, el, rest...); }
};

inline bool parseHelper(std::string cmdName, Outputter* out, tok_iter& token, const tok_iter& end) 
{
	if (token != end) return false;
	else return true;
}

template <typename T, typename... Rest>
bool parseHelper(std::string name, Outputter* out, tok_iter& token, 
		const tok_iter& end, T& el, Rest&... rest) 
{
	if (token == end)
		{ out->writeError(std::string("Too few arguments to ") + name); return false; }
	else if (!parseElement(token, end, el)) return false;
	else return parseHelper(name, out, ++token, end, rest...);
}



#endif // CMD_PARSE_H
