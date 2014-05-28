#ifndef TYPES_H
#define TYPES_H

#include "vector2d.h"

#include <graph.h>
#include <gtkmm.h>
#include <cairomm/context.h>

#include <map>
#include <string>
#include <memory>
using std::unique_ptr;

struct BoundingBox
{
	BoundingBox(double t, double l, double b, double r) : top(t), left(l), bottom(b), right(r) { }
	BoundingBox(const Vector2D& tl, const Vector2D& br) : 
		top(tl.y), left(tl.x), bottom(br.y), right(br.x) { }
	double top, left, bottom, right;
	double width() const { return right - left; }
	double height() const { return bottom - top; }
};

enum BranchDir { Down = -1, Up = 1 };

typedef Cairo::RefPtr<Cairo::Context> CairoContext;
typedef unique_ptr<Gtk::Widget> GtkWidgetPtr;
typedef unique_ptr<graph::Graph> GraphPtr;

const int VISIBLE = 1 << 0;
const int MOVABLE = 1 << 1;

// Sigh... boost...
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::escaped_list_separator<char>>::iterator tok_iter;

#endif // TYPES_H



