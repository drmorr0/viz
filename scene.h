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

	int id() { return mId; }
	int state() { return mState; }

	virtual bool contains(const Vector2D& pt) = 0;
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) = 0;
	virtual void move(const Vector2D& delta) = 0;
	virtual void displayInfo() = 0;

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
	std::vector<int> findObjects(const Vector2D& pt);
	SceneObject* const get(int id) const;

	void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom);

private:
	// Disable copy/assignment of Scenes for right now (TODO)
	Scene(const Scene&); 
	const Scene operator=(const Scene&);

	int mObjNextId;
	std::map<int, SceneObject*> mObjects;
};

#endif // SCENE_H
