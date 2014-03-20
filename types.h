#ifndef TYPES_H
#define TYPES_H

#include <graph.h>
#include <gtkmm.h>
#include <cairomm/context.h>
#include <string>
#include <memory>
using std::unique_ptr;

enum BranchDir { Down = -1, Up = 1 };

struct BnbVertexData : public graph::VertexData
{
	int expTime;
	int pruneTime;
	int genTime;
	double lb, ub;
	double estimate;
	int contour;
	std::string branchVar;
	BranchDir branchDir;

	BnbVertexData* clone() { return new BnbVertexData(*this); }
	~BnbVertexData() { }
};

typedef Glib::RefPtr<Gtk::Builder> GtkBuildPtr;
typedef Cairo::RefPtr<Cairo::Context> CairoContext;
typedef unique_ptr<Gtk::Widget> GtkWidgetPtr;
typedef unique_ptr<graph::Graph> GraphPtr;

const int MOVABLE = 1 << 0;

#endif // TYPES_H



