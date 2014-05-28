// scene_obj.cpp: David R. Morrison, March 2014
// Implementation details for SceneObjects

#include "vertex_obj.h"
#include "vector2d.h"
#include "util.h"

using namespace std;

VertexSceneObject::VertexSceneObject(double x, double y, double radius, double thickness,
		const Gdk::Color& color, const Gdk::Color& fillColor) :
	VertexSceneObject(Vector2D(x, y), radius, thickness, color, fillColor)
{
	/* Do nothing */
}

VertexSceneObject::VertexSceneObject(const Vector2D& center, double radius, double thickness,
		const Gdk::Color& color, const Gdk::Color& fillColor) :
	SceneObject(MOVABLE | VISIBLE),
	mCenter(center),
	mRadius(radius),
	mThickness(thickness),
	mColor(color),
	mFill(fillColor),
	mLabel("Hi!")
{
	/* Do nothing */
}

bool VertexSceneObject::contains(const Vector2D& pt) const
{
	return length(mCenter - pt) < mRadius;
}

bool VertexSceneObject::inside(const BoundingBox& area) const
{
	return (mCenter.x > area.left &&
			mCenter.x < area.right &&
			mCenter.y > area.top &&
			mCenter.y < area.bottom);
}

void VertexSceneObject::render(const CairoContext& ctx, const Vector2D& canvOffset, 
		double zoom) const
{
	// Translate from the position in graph-embedding space to canvas space
	Vector2D canvPos = canvOffset + zoom * mCenter;
	double canvRadius = zoom * mRadius;

	// Draw a circle for the node
	ctx->save();
	ctx->arc(canvPos.x, canvPos.y, canvRadius, 0, 2 * M_PI);
	ctx->set_source_rgb(mFill.get_red_p(), mFill.get_green_p(), mFill.get_blue_p());
	ctx->fill_preserve();
	ctx->set_source_rgb(mColor.get_red_p(), mColor.get_green_p(), mColor.get_blue_p());
	ctx->set_line_width(mThickness);
	ctx->stroke();

	// Add a node label
	if (!mLabel.empty())
	{
		Cairo::TextExtents textBounds; 

		// I don't really know what the ToyFontFace does, but this says to use the default system
		// font with normal slant and weight
		Cairo::RefPtr<Cairo::ToyFontFace> font =
			Cairo::ToyFontFace::create("", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);

		// We need to figure out the bounding box for the text so we can center it in the node
		ctx->set_font_face(font);
		ctx->set_font_size(canvRadius);
		ctx->get_text_extents(mLabel, textBounds);

		// Move to the center of the node and add the label
		ctx->move_to(canvPos.x - (textBounds.width / 2), canvPos.y + (textBounds.height / 2));
		ctx->show_text(mLabel);
		ctx->begin_new_path();
	}

	ctx->restore();
}

void VertexSceneObject::move(const Vector2D& delta)
{
	mCenter += delta;
}

BoundingBox VertexSceneObject::bounds() const
{ 
	double absRadius = getAbsRadius();
	return BoundingBox(mCenter - Vector2D(absRadius, absRadius), 
					   mCenter + Vector2D(absRadius, absRadius));
}


