// scene_obj.cpp: David R. Morrison, March 2014
// Implementation details for SceneObjects

#include "scene.h"
#include "edge_obj.h"
#include "vertex_obj.h"
#include "vector2d.h"
#include "util.h"

using namespace std;

EdgeSceneObject::EdgeSceneObject(int tailId, int headId) :
	SceneObject(VISIBLE),
	mTailId(tailId),
	mHeadId(headId)
{
	/* Do nothing */
}

bool EdgeSceneObject::contains(const Vector2D& pt) const
{
	return false; // TODO
}

bool EdgeSceneObject::inside(const BoundingBox& area) const
{
	return false; // TODO
}

/* 
 * Render an edge of the graph; edges are always attached to two vertices, and we want to draw a
 * line from the center of one vertex to the center of the other, but we want to take into account
 * the radius of the vertex.  We need to do a bit of vector math here to make this work right
 */
void EdgeSceneObject::render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) const
{
	// Get the Scene ids of the two endpoints
	VertexSceneObject* headObj = (VertexSceneObject*)mfpParentScene->get(mHeadId);
	VertexSceneObject* tailObj = (VertexSceneObject*)mfpParentScene->get(mTailId);

	// Only draw an edge if both endpoints are marked as visible
	if ((headObj->state() & VISIBLE) != VISIBLE || 
		(tailObj->state() & VISIBLE) != VISIBLE) 
		return;

	// The canvas position of the endpoints is the absolute position, adjusted by the current zoom
	// level and the current offset of the canvas.  We can compute a vector that connects the
	// centers of the two endpoints by subtracting one from the other.  
	Vector2D headCanvPos = canvOffset + zoom * headObj->getPos();
	Vector2D tailCanvPos = canvOffset + zoom * tailObj->getPos();
	Vector2D connector = tailCanvPos - headCanvPos;
	normalize(connector); 
	
	// The actual radius of the objects needs to consider both the radius and the thickness of the
	// stroke.  We divide by 2 because the stroke starts at the center and goes out equally in both
	// directions
	double headCanvRadius = zoom * (headObj->getRadius() + headObj->getThickness() / 2);
	double tailCanvRadius = zoom * (tailObj->getRadius() + tailObj->getThickness() / 2);

	// The normalized connection vector can then be multiplied by the radius of the nodes to get the
	// start and end positions where we should be drawing
	Vector2D edgeStart = -1 * tailCanvRadius * connector;
	Vector2D edgeEnd = headCanvRadius * connector;

	ctx->save();
	auto gradient = Cairo::LinearGradient::create(tailCanvPos.x + edgeStart.x, 
												  tailCanvPos.y + edgeStart.y,
												  headCanvPos.x + edgeEnd.x,
												  headCanvPos.y + edgeEnd.y);
	Gdk::Color tColor = tailObj->getColor(); Gdk::Color hColor = headObj->getColor();
	gradient->add_color_stop_rgb(0, tColor.get_red_p(), tColor.get_green_p(), tColor.get_blue_p());
	gradient->add_color_stop_rgb(1, hColor.get_red_p(), hColor.get_green_p(), hColor.get_blue_p());
	ctx->set_source(gradient);
	ctx->move_to(tailCanvPos.x + edgeStart.x, tailCanvPos.y + edgeStart.y);
	ctx->line_to(headCanvPos.x + edgeEnd.x, headCanvPos.y + edgeEnd.y);
	ctx->stroke();
	ctx->restore();
}

BoundingBox EdgeSceneObject::bounds() const
{
	// Every edge is attached to a vertex on both ends, so it will never be the top left point
	return { 0, 0, 0, 0 };
}

