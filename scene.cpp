// scene.cpp: David R. Morrison, March 2014
// Class to manage the scene for the visualization software

#include "scene.h"

using namespace std;

Scene::~Scene()
{
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
		delete *i;
}

int Scene::addObject(SceneObject* obj)
{
	obj->mId = mObjects.size();
	mObjects.push_back(obj);
	return obj->mId;
}

vector<int> Scene::findObjects(const Vector2D& pt)
{
	vector<int> objs;
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
	{
		if ((*i)->contains(pt))
			objs.push_back((*i)->mId);
	}

	return objs;
}

void Scene::render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom)
{
	for (auto i = mObjects.begin(); i != mObjects.end(); ++i)
		(*i)->render(ctx, canvOffset, zoom);
}


