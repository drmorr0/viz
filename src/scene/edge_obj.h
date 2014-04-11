#ifndef EDGE_OBJ_H
#define EDGE_OBJ_H
/* 
 * edge_obj.h: David R. Morrison, March 2014
 *
 * A scene object representing an edge between two nodes of a graph
 *
 */

#include "scene_obj.h"

// A scene object for edges of a graph
class EdgeSceneObject : public SceneObject
{
public:
	EdgeSceneObject(int tailId, int headId);

	virtual bool contains(const Vector2D& pt) const;
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) const;
	virtual void move(const Vector2D& delta) { return; }
	virtual BoundingBox bounds() const;

private:
	int mTailId, mHeadId;
};

#endif // EDGE_OBJ_H
