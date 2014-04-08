#ifndef SCENE_H
#define SCENE_H
/*
 * scene_graph.h: David R. Morrison, March 2014
 *
 * The SceneGraph class contains all the functions necessary to display a scene in the visualization
 * toolkit.  This includes things like determining objects that were clicked on by the mouse,
 * determining the currently-visible area, etc.  In order to handle potentially very large scenes,
 * this class should be as efficient as possible.
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
	Scene* mParentScene;
	int mState;

private:		// Only the Scene Manager should be able to control the object's id
	int mId;
};

class Scene
{
public:
	Scene() : mObjNextId(0) { }
	~Scene();

	int addObject(SceneObject* obj); 
	std::vector<SceneObject*> findObjects(const Vector2D& pt) const;
	SceneObject* get(int id) const;
	BoundingBox bounds() const;

	void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom);
	void showAll();

private:
	// Disable copy/assignment of Scenes for right now (TODO)
	Scene(const Scene&); 
	const Scene operator=(const Scene&);

	int mObjNextId;
	std::map<int, SceneObject*> mObjects;
};

#endif // SCENE_H
