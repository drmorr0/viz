#ifndef TYPES_H
#define TYPES_H

#include <graph.h>
#include <gtkmm.h>
#include <cairomm/context.h>

#include <map>
#include <string>
#include <memory>
using std::unique_ptr;

enum BranchDir { Down = -1, Up = 1 };

typedef Cairo::RefPtr<Cairo::Context> CairoContext;
typedef unique_ptr<Gtk::Widget> GtkWidgetPtr;
typedef unique_ptr<graph::Graph> GraphPtr;

enum MatchOp { Less, LessEq, Eq, NotEq, GreaterEq, Greater };

const int VISIBLE = 1 << 0;
const int MOVABLE = 1 << 1;

#endif // TYPES_H



