// scene_obj.cpp: David R. Morrison, March 2014
// Implementation details for SceneObjects

#include "scene_obj.h"

VertexSceneObject::VertexSceneObject(double x, double y, double radius) :
	VertexSceneObject(Vector2D(x, y), radius)
{
	/* Do nothing */
}

VertexSceneObject::VertexSceneObject(const Vector2D& center, double radius) :
	SceneObject(MOVABLE | VISIBLE),
	mCenter(center),
	mRadius(radius)
{
	/* Do nothing */
}

bool VertexSceneObject::contains(const Vector2D& pt)
{
	return length(mCenter - pt) < mRadius;
}

void VertexSceneObject::render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom)
{
	// Translate from the position in graph-embedding space to canvas space
	Vector2D canvPos = canvOffset + zoom * mCenter;
	double canvRadius = zoom * mRadius;

	// Draw a circle for the node
	ctx->arc(canvPos.x, canvPos.y, canvRadius, 0, 2 * M_PI);
	ctx->stroke();
}

void VertexSceneObject::move(const Vector2D& delta)
{
	mCenter += delta;
}

EdgeSceneObject::EdgeSceneObject(int tailId, int headId) :
	SceneObject(VISIBLE),
	mTailId(tailId),
	mHeadId(headId)
{
	/* Do nothing */
}

bool EdgeSceneObject::contains(const Vector2D& pt)
{
	return false; // TODO
}

/* 
 * Render an edge of the graph; edges are always attached to two vertices, and we want to draw a
 * line from the center of one vertex to the center of the other, but we want to take into account
 * the radius of the vertex.  We need to do a bit of vector math here to make this work right
 */
void EdgeSceneObject::render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom)
{
	// Get the Scene ids of the two endpoints
	VertexSceneObject* headObj = (VertexSceneObject*)mParentScene->get(mHeadId);
	VertexSceneObject* tailObj = (VertexSceneObject*)mParentScene->get(mTailId);

	// Only draw an edge if both endpoints are marked as visible
	if ((headObj->state() & VISIBLE) != VISIBLE || 
		(tailObj->state() & VISIBLE) != VISIBLE) 
		return;

	// The canvas position of the endpoints is the absolute position, adjusted by the current zoom
	// level and the current offset of the canvas.  We can compute a vector that connects the
	// centers of the two endpoints by subtracting one from the other
	Vector2D headCanvPos = canvOffset + zoom * headObj->getPos();
	double headCanvRadius = zoom * headObj->getRadius();
	Vector2D tailCanvPos = canvOffset + zoom * tailObj->getPos();
	double tailCanvRadius = zoom * tailObj->getRadius();
	Vector2D connector = tailCanvPos - headCanvPos;
	normalize(connector); 

	// The normalized connection vector can then be multiplied by the radius of the nodes to get the
	// start and end positions where we should be drawing
	Vector2D edgeStart = headCanvRadius * connector;
	Vector2D edgeEnd = -1 * tailCanvRadius * connector;

	ctx->move_to(headCanvPos.x + edgeStart.x, headCanvPos.y + edgeStart.y);
	ctx->line_to(tailCanvPos.x + edgeEnd.x, tailCanvPos.y + edgeEnd.y);
	ctx->stroke();
}

