#ifndef SCENE_OBJ_H
#define SCENE_OBJ_H
/*
 * scene_obj.h: David R. Morrison, March 2014
 *
 * An abstract base class for an object that belongs to a scene graph
 *
 */

#include "vector2d.h"
#include "types.h"

#include <map>

class Scene;

// A generic SceneObject base class
class SceneObject
{
public:
	SceneObject(int state) : mState(state) { };
	virtual ~SceneObject() { };

	int id() const { return mId; }
	int& state() { return mState; }

	virtual bool contains(const Vector2D& pt) const = 0;
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) const= 0;
	virtual void move(const Vector2D& delta) = 0;
	virtual BoundingBox bounds() const = 0;

protected:
	friend class Scene;
	Scene* mfpParentScene;
	int mState;

private:		// Only the Scene Manager should be able to control the object's id
	int mId;
};


#endif // SCENE_OBJ_H
