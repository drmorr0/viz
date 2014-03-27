// scene.cpp: David R. Morrison, March 2014
// Class to manage the scene for the visualization software

#include "scene.h"
#include "scene_obj.h"

using namespace std;

Scene::~Scene()
{
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
		delete (i->second);
}

int Scene::addObject(SceneObject* obj)
{
	obj->mParentScene = this;
	obj->mId = mObjNextId++;
	mObjects[obj->mId] = obj;
	return obj->mId;
}

vector<SceneObject*> Scene::findObjects(const Vector2D& pt)
{
	vector<SceneObject*> objs;
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
	{
		if (i->second->contains(pt))
			objs.push_back(i->second);
	}

	return objs;
}

SceneObject* const Scene::get(int id) const
{
	if (mObjects.count(id) == 0) return nullptr;
	else return mObjects.find(id)->second;
}

void Scene::render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom)
{
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
		i->second->render(ctx, canvOffset, zoom);
}


