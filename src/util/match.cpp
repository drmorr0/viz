// match.cpp: David R. Morrison, May 2014
// Implementation for the Match class

#include "match.h"
#include "more_graph_utils.h"

using namespace std;

Match::Match(const Graph& g) :
	mGraph(g)
{
	/* Do nothing */
}

bool Match::setFilter(const string& filter)
{
	mFilter = filter;
	return true;
}

bool Match::setOperator(const string& oper)
{
    if      (oper == "<")  mOper = Less;
    else if (oper == "<=") mOper = LessEq;
    else if (oper == "==") mOper = Eq;
    else if (oper == "!=") mOper = NotEq;
    else if (oper == ">=") mOper = GreaterEq;
    else if (oper == ">")  mOper = Greater;
    else return false; 

	return true;
}

bool Match::setValue(const string& value)
{
	try 
	{ 
		mNumericValue = stod(value); 
		mIsNumeric = true;
	}
	catch (invalid_argument& e)
	{
		mTextValue = value;
		mIsNumeric = false;
	}
	return true;
}

vector<int> Match::verts()
{
	if (mIsNumeric)
		return graph::match(mGraph, mFilter, mOper, mNumericValue);
	else return vector<int>(); // TODO
}
