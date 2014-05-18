// cmd_parse.cpp: David R. Morrison
// Helper routines for parsing command line arguments

#include "cmd_parse.h"

#include <string>

using namespace std;

bool parseElement(tok_iter& token, const tok_iter& end, int& el)
{
	printf("Got an int!\n");
	return true;
}

bool parseElement(tok_iter& token, const tok_iter& end, string& el)
{
	printf("Got a string!\n");
	return true;
}
