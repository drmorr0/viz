#ifndef MATCH_H
#define MATCH_H
/*
 * match.h: David R. Morrison, 28 May 2014
 *
 * The match object allows for complex specifications against a set of vertices in a given
 * graph
 */

#include <vector>
#include <string>
#include <graph.h>

using graph::Graph;

class Match 
{ 
public: 
	enum Operator { Less, LessEq, Eq, NotEq, GreaterEq, Greater };

	Match(const Graph& g);
	bool setFilter(const std::string& filter);
	bool setOperator(const std::string& oper);
	bool setValue(const std::string& value);
	std::vector<int> verts(); 

private:
	Graph mGraph;
	std::string mFilter;
	Operator mOper;
	double mNumericValue;
	std::string mTextValue;
	bool mIsNumeric;
};

#endif // MATCH_H
