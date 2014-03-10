// vertex.cpp: David R. Morrison, March 2014
// Implementation details for the VertexSceneObject class

#include "vertex.h"

VertexSceneObject::VertexSceneObject(double x, double y, double radius) :
	VertexSceneObject(Vector2D(x, y), radius)
{
	/* Do nothing */
}

VertexSceneObject::VertexSceneObject(const Vector2D& center, double radius) :
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
