#ifndef GRAPH_IO_H
#define GRAPH_IO_H
/*
 * graph_io.h: David R. Morrison
 *
 * Functions for input and output for Graphs
 */

namespace drm
{

class Graph;

void readDIMACS(const char* filename, Graph& g);
void readJsonTree(const char* filename, Graph& g);
void print(const Graph& g);
void printShort(const Graph& g);

}; // namespace drm

#endif // GRAPH_IO_H

