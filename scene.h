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

#include <vector>

class SceneObject
{
public:
	virtual ~SceneObject() { };
	virtual bool contains(const Vector2D& pt) = 0;
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) = 0;

private:
	friend class Scene;
	int mId;
};

class Scene
{
public:
	Scene() { }
	~Scene();
	int addObject(SceneObject* obj); 
	std::vector<int> findObjects(const Vector2D& pt);
	void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom);

private:
	// Disable copy/assignment of Scenes for right now (TODO)
	Scene(const Scene&); 
	const Scene operator=(const Scene&);

	std::vector<SceneObject*> mObjects;
};

#endif // SCENE_H
