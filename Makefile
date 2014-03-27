# To use this makefile, do the following:
# 1. Update $(SRCS) to reflect the list of files you want to compile
# 2. Set $(OBJDIR) to the directory in which you want the intermediate files to be placed (and 
#    make sure the directory exists)
# 3. Change $(EXEC) to be the name you want for your executable

SRCS = main.cpp scene.cpp scene_obj.cpp viz_io.cpp viz_canvas.cpp viz_window.cpp viz_cmd.cpp\
	   vector2d.cpp more_graph_utils.cpp 
CFLAGS = `pkg-config --cflags cairomm-1.0 gtkmm-3.0` -I../graph-lib
LDFLAGS = `pkg-config --libs cairomm-1.0 gtkmm-3.0` -ljson_spirit -L../graph-lib 
EXEC = gviz

# You can leave this stuff alone for the most part; it sets the right C++ standard, tells the
# compiler to print output nicely, and prints some useful warning messages

CC = g++ 
STD = -std=c++0x
FORMAT = -fno-pretty-templates
WARNINGS = -Wempty-body -Wall -Wno-sign-compare
DEBUGFLAGS = -g -pg
OPTFLAGS = -O2
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))
DOBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.debug))
EXECD = $(EXEC)_d

.DEFAULT_GOAL := $(EXEC)
.PHONY: all clean debug test

$(OBJDIR)/%.o : %.cpp $(OBJDIR)/%.d
	@echo; echo "Compiling $@"; echo "---"
	@$(CC) $(STD) $(WARNINGS) $(FORMAT) -o $@ $(CFLAGS) $(OPTFLAGS) -c $<

$(OBJDIR)/%.debug : %.cpp $(OBJDIR)/%.d
	@echo; echo "Compiling debug version: $@"; echo "---"
	@$(CC) $(STD) $(WARNINGS) $(FORMAT) -o $@ $(CFLAGS) $(DEBUGFLAGS) -c $<

# Use the -MM option for g++ or gcc to automatically determine dependency structure for $(SRCS).
# This will get stuck into a $(OBJDIR)/<sourcename>.d file, which the object files depend on.  Then,
# whenever any file in the dependency structure changes, we'll rebuild and remake.  Slick!
$(OBJDIR)/%.d : %.cpp 
	@$(SHELL) -ec "$(CC) $(STD) -MM $(CFLAGS) $< | \
		sed 's/\($*\)\.o[ :]*/$(OBJDIR)\/\1.o $(OBJDIR)\/\1.debug : /g' > $@; [ -s $@ ] || rm -f $@"

all: $(EXEC) $(EXECD)

debug: $(EXECD)

$(EXEC): $(OBJS)
	@echo; echo "Linking $(EXEC)"
	@$(CC) $(STD) $(FORMAT) -o $(EXEC) $(OPTFLAGS) $(OBJS) $(LDFLAGS) -lgraph
	@echo "Done."

$(EXECD): $(DOBJS)
	@echo; echo "Linking debug version $(EXECD)"
	@$(CC) $(STD) $(FORMAT) -o $(EXECD) $(DEBUGFLAGS) $(DOBJS) $(LDFLAGS) -lgraph_d
	@echo "Done."

clean:
	-rm $(EXEC) $(EXECD) $(OBJDIR)/*.d $(OBJDIR)/*.debug $(OBJDIR)/*.o gmon.out graph_test;

TESTSRC = graph_test.cpp graph.cpp graph_utils.cpp graph_order.cpp
TESTOBJS = $(addprefix $(OBJDIR)/,$(TESTSRC:.cpp=.debug))
test: $(TESTOBJS)
	$(CC) $(STD) $(FORMAT) -o graph_test -pg $(TESTOBJS)

include $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.d))

