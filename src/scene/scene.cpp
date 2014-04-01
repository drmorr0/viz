// scene.cpp: David R. Morrison, March 2014
// Class to manage the scene for the visualization software

#include "scene.h"
#include "scene_obj.h"

using namespace std;

// Delete everything in the scene graph
Scene::~Scene()
{
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
		delete (i->second);
}

// Add an object to the scene graph and return its id
int Scene::addObject(SceneObject* obj)
{
	obj->mParentScene = this;
	obj->mId = mObjNextId++;
	mObjects[obj->mId] = obj;
	return obj->mId;
}

// Find all objects that contain a particular point
vector<SceneObject*> Scene::findObjects(const Vector2D& pt) const
{
	vector<SceneObject*> objs;
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
	{
		if (i->second->contains(pt))
			objs.push_back(i->second);
	}

	return objs;
}

// Get an object by its ID
SceneObject* Scene::get(int id) const
{
	if (mObjects.count(id) == 0) return nullptr;
	else return mObjects.find(id)->second;
}

// Render all visible objects (TODO what about objects off-screen?)
void Scene::render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom)
{
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
	{
		if (i->second->state() & VISIBLE)
			i->second->render(ctx, canvOffset, zoom);
	}
}

// Show all objects in the scene graph
void Scene::showAll()
{
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
		i->second->state() |= VISIBLE;
}


