// scene.cpp: David R. Morrison, March 2014
// Class to manage the scene for the visualization software

#include "scene.h"
#include "scene_obj.h"
#include "util.h"

using namespace std;

// Delete everything in the scene graph
Scene::~Scene()
{
	for (auto i = mpObjects.begin(); i != mpObjects.end(); ++i)
		delete (i->second);
}

// Add an object to the scene graph and return its id
int Scene::addObject(SceneObject* obj)
{
	obj->mfpParentScene = this;
	obj->mId = mObjNextId++;
	mpObjects[obj->mId] = obj;
	return obj->mId;
}

// Find all objects that contain a particular point
vector<SceneObject*> Scene::findObjects(const Vector2D& pt) const
{
	vector<SceneObject*> objs;
	for (auto i = mpObjects.begin(); i != mpObjects.end(); ++i)
	{
		if (i->second->contains(pt))
			objs.push_back(i->second);
	}

	return objs;
}

// Find all objects that are contained within a bounding box
vector<SceneObject*> Scene::findObjects(const BoundingBox& area) const
{
	vector<SceneObject*> objs;
	for (auto i = mpObjects.begin(); i != mpObjects.end(); ++i)
	{
		if (i->second->inside(area))
			objs.push_back(i->second);
	}

	return objs;
}

// Get an object by its ID
SceneObject* Scene::get(int id) const
{
	if (mpObjects.count(id) == 0) return nullptr;
	else return mpObjects.find(id)->second;
}

BoundingBox Scene::bounds() const
{
	// TODO there's probably a better way to do this
	BoundingBox box(Infinity, Infinity, -Infinity, -Infinity);
	for (auto i = mpObjects.begin(); i != mpObjects.end(); ++i)
	{
		if (i->second->state() & VISIBLE)
		{
			if (i->second->bounds().top < box.top)
				box.top = i->second->bounds().top;
			if (i->second->bounds().left < box.left)
				box.left = i->second->bounds().left;
			if (i->second->bounds().bottom > box.bottom)
				box.bottom = i->second->bounds().bottom;
			if (i->second->bounds().right > box.right)
				box.right = i->second->bounds().right;

		}
	}

	return box;
}

// Render all visible objects (TODO what about objects off-screen?)
void Scene::render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom)
{
	for (auto i = mpObjects.begin(); i != mpObjects.end(); ++i)
	{
		if (i->second->state() & VISIBLE)
			i->second->render(ctx, canvOffset, zoom);
	}
}

// Show all objects in the scene graph
void Scene::showAll()
{
	for (auto i = mpObjects.begin(); i != mpObjects.end(); ++i)
		i->second->state() |= VISIBLE;
}


