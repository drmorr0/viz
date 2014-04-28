#ifndef SCENE_H
#define SCENE_H
/*
 * scene.h: David R. Morrison, March 2014
 *
 * The Scene class contains all the functions necessary to display a scene in the visualization
 * toolkit.  This includes things like determining objects that were clicked on by the mouse,
 * determining the currently-visible area, etc.  In order to handle potentially very large scenes,
 * this class should be as efficient as possible.
 *
 */

#include "vector2d.h"
#include "types.h"

#include <map>

class Scene;
class SceneObject;

class Scene
{
public:
	Scene() : mObjNextId(0) { }
	~Scene();

	int addObject(SceneObject* obj); 
	std::vector<SceneObject*> findObjects(const Vector2D& pt) const;
	std::vector<SceneObject*> findObjects(const BoundingBox& area) const;
	SceneObject* get(int id) const;
	BoundingBox bounds() const;

	void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom);
	void showAll();

private:
	std::map<int, SceneObject*> mpObjects;
	int mObjNextId;

	// Disable copy/assignment of Scenes for right now (TODO)
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
};

#endif // SCENE_H
