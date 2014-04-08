cmake_minimum_required(VERSION 2.8)

project(VIZ)

# Output in the root directory -- need to adjust location of layout files in src/util/builder.h
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VIZ_SOURCE_DIR})

# Need to do something for windows here
set(CMAKE_CXX_FLAGS "-Wempty-body -Wall -Wno-sign-compare -std=c++0x ${CMAKE_CXX_FLAGS}")

# The root location for the source code
set(VIZ_CODE_DIR "${VIZ_SOURCE_DIR}/src")

# Custom include directories
include_directories(
	"${VIZ_CODE_DIR}"
	"${VIZ_CODE_DIR}/cmd"
	"${VIZ_CODE_DIR}/gtk"
	"${VIZ_CODE_DIR}/io"
	"${VIZ_CODE_DIR}/scene"
	"${VIZ_CODE_DIR}/util"
)

# Set the name of the executable output with source file dependencies
add_executable(viz
	"${VIZ_CODE_DIR}/main.cpp"
	"${VIZ_CODE_DIR}/cmd/cmd.cpp"
	"${VIZ_CODE_DIR}/cmd/format.cpp"
	"${VIZ_CODE_DIR}/cmd/hide.cpp"
	"${VIZ_CODE_DIR}/cmd/help.cpp"
	"${VIZ_CODE_DIR}/cmd/load.cpp"
	"${VIZ_CODE_DIR}/cmd/save.cpp"
	"${VIZ_CODE_DIR}/cmd/show.cpp"
	"${VIZ_CODE_DIR}/cmd/quit.cpp"
	"${VIZ_CODE_DIR}/gtk/viz_canvas.cpp"
	"${VIZ_CODE_DIR}/gtk/viz_prompt.cpp"
	"${VIZ_CODE_DIR}/gtk/viz_window.cpp"
	"${VIZ_CODE_DIR}/io/viz_io.cpp"
	"${VIZ_CODE_DIR}/scene/scene.cpp"
	"${VIZ_CODE_DIR}/scene/scene_obj.cpp"
	"${VIZ_CODE_DIR}/util/builder.cpp"
	"${VIZ_CODE_DIR}/util/more_graph_utils.cpp"
	"${VIZ_CODE_DIR}/util/vector2d.cpp"
)

# This is a hack, since both graph-lib and this project use a util.h file; loading these libraries
# now ensures that our util.h is on the include before graph-lib.  I should really change or do
# something about that...

# Set this to the location of the graph library on your configuration
set(GRAPHLIB_DIR "${VIZ_SOURCE_DIR}/../graph-lib/")
add_library(graph STATIC IMPORTED)
add_library(graph_d STATIC IMPORTED)
set_property(TARGET graph PROPERTY IMPORTED_LOCATION "${GRAPHLIB_DIR}/libgraph.a")
set_property(TARGET graph_d PROPERTY IMPORTED_LOCATION "${GRAPHLIB_DIR}/libgraph_d.a")
include_directories("${GRAPHLIB_DIR}")

# Load gtkmm and cairomm (TODO windows)
find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)
include_directories(${GTKMM_INCLUDE_DIRS} ${CAIROMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS} ${CAIROMM_LIBRARY_DIRS})

target_link_libraries(viz optimized graph)
target_link_libraries(viz debug graph_d)
target_link_libraries(viz ${GTKMM_LIBRARIES} ${CAIROMM_LIBRARIES} "-ljson_spirit")






