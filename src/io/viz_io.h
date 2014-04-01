#ifndef VIZ_IO_H
#define VIZ_IO_H
/*
 * viz_io.h: David R. Morrison
 *
 * Functions for input and output for the visualization software
 */

#include <graph.h>
using graph::Graph;

Graph readJsonTree(const char* filename);

#endif // VIZ_IO_H 

