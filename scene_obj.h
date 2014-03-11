#ifndef SCENE_OBJ_H
#define SCENE_OBJ_H
/* 
 * scene_obj.h: David R. Morrison, March 2014
 *
 * Contains definitions for the various types of objects that can appear in a scene (TODO will this
 * need to get split out into separate files at some point?)
 *
 */

#include "scene.h"

// A generic SceneObject base class
class SceneObject
{
public:
	virtual ~SceneObject() { };
	virtual bool contains(const Vector2D& pt) = 0;
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) = 0;

protected:
	friend class Scene;
	Scene* mParentScene;

private:		// Only the Scene Manager should be able to control the object's id
	int mId;
};

// A scene object for vertices of a graph
class VertexSceneObject : public SceneObject
{
public:
	VertexSceneObject(double x, double y, double radius);
	VertexSceneObject(const Vector2D& center, double radius);

	virtual bool contains(const Vector2D& pt);
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom);

	Vector2D getPos() const { return mCenter; }
	double getRadius() const { return mRadius; }

private:
	Vector2D mCenter;
	double mRadius;
};

// A scene object for edges of a graph
class EdgeSceneObject : public SceneObject
{
public:
	EdgeSceneObject(int tailId, int headId);

	virtual bool contains(const Vector2D& pt);
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom);

private:
	int mTailId, mHeadId;
};

#endif // SCENE_OBJ_H
