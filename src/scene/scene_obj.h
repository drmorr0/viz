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

// A scene object for vertices of a graph
class VertexSceneObject : public SceneObject
{
public:
	VertexSceneObject(double x, double y, double radius = 10, double thickness = 2, 
			const Gdk::Color& color = Gdk::Color("#000000"), 
			const Gdk::Color& fillColor = Gdk::Color("#ffffff"));
	VertexSceneObject(const Vector2D& center, double radius = 10, double thickness = 2, 
			const Gdk::Color& color = Gdk::Color("#000000"), 
			const Gdk::Color& fillColor = Gdk::Color("#ffffff"));

	virtual bool contains(const Vector2D& pt) const;
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) const;
	virtual void move(const Vector2D& delta);
	virtual BoundingBox bounds() const;

	Vector2D getPos() const { return mCenter; }
	void setFill(const Gdk::Color& c) { mFill = c; }
	Gdk::Color getFill() const { return mFill; }
	void setColor(const Gdk::Color& c) { mColor = c; }
	Gdk::Color getColor() const { return mColor; }
	void setRadius(double r) { mRadius = r; }
	double getRadius() const { return mRadius; }
	void setThickness(double t) { mThickness = t; }
	double getThickness() const { return mThickness; }
	double getAbsRadius() const { return mRadius + mThickness; }

private:
	Vector2D mCenter;
	double mRadius;
	double mThickness;
	Gdk::Color mColor;
	Gdk::Color mFill;
};

// A scene object for edges of a graph
class EdgeSceneObject : public SceneObject
{
public:
	EdgeSceneObject(int tailId, int headId);

	virtual bool contains(const Vector2D& pt) const;
	virtual void render(const CairoContext& ctx, const Vector2D& canvOffset, double zoom) const;
	virtual void move(const Vector2D& delta) { return; }
	virtual BoundingBox bounds() const;

private:
	int mTailId, mHeadId;
};

#endif // SCENE_OBJ_H
