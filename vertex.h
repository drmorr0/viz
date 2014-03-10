/* 
 * vertex.h: David R. Morrison, March 2014
 *
 * The VertexSceneObject is a SceneObject that represents the vertex of a graph
 *
 */

#include "scene.h"

class VertexSceneObject : public SceneObject
{
public:
	VertexSceneObject(double x, double y, double radius);
	VertexSceneObject(const Vector2D& center, double radius);

	virtual bool contains(const Vector2D& pt);
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom);

private:
	Vector2D mCenter;
	double mRadius;
};
