// more_graph_utils.cpp: David R. Morrison
// Additional utility functions for working with graphs

#include "more_graph_utils.h"
#include "util.h"

#include <graph.h>

using namespace std;

namespace graph
{

/*
 * Loop through all vertices of the specified graph and return a list of the ones that match
 * the specified rule.  I'm not exactly sure how efficient this is going to be for large graphs...
 */
vector<int> match(const Graph& g, const string& filterText, Match::Operator op, double value)
{
	vector<int> matched;
	for (auto v = g.begin(); v != g.end(); ++v)
	{
		graph::VertexData* data = g.vertexData(v->first);
		if (!data) continue;	// If a vertex has no data associated, skip it

		for (auto prop = data->properties.begin(); prop != data->properties.end(); ++prop)
		{
			// Check to see if the property contains the specified filter text.  Right now it must
			// match exactly, though being able to do something like a regex might be cool...
			if (prop->first.find(filterText) != string::npos)
			{
				double propVal;
				try { propVal = stod(prop->second); } catch (invalid_argument& e) { continue; }

				// How to handle non-numeric properties? TODO
				if ((op == Match::Less 	 	&& propVal <  value) ||
					(op == Match::LessEq	&& propVal <= value) ||
					(op == Match::Eq		&& propVal == value) ||
					(op == Match::NotEq	 	&& propVal != value) ||
					(op == Match::GreaterEq && propVal >= value) ||
					(op == Match::Greater	&& propVal >  value))
					matched.push_back(v->first);
			}
		}
	}

	return matched;
}

/*
 * This is a better graph layout function than the naive layout function used earlier;
 * it behaves similarly to the old function, but has a better bounding box: instead of
 * using a rectangular bounding box, it uses a trapezoidal bounding box to more closely
 * approximate the shape of the current subtree.  This allows it to detect when some 
 * subtrees could be pushed closer together (horizontally) without overlapping.  I think
 * there may be some circumstances when it still causes overlap, but it appears to work
 * quite well in most cases
 *
 * This is the bounding trapezoid:
 *       1    2
 *    /-----|---\
 *  6/      |5   \7
 *  /-------|-----\
 *       3     4
 *  lTop is the length of segment 1; rTop is the length of segment 2;
 *  lBase is the length of segment 3; rBase is the length of segment 4;
 *  height is the length of segment 5;
 *
 *  The x coordinate of the subtree's root is positioned at the vertical line segment 5, and
 *  the y coordinate is positioned so that the edge of the root touches (1,2,5)
 */
struct SubtreeBounds
{
	double lTop, rTop;
	double lBase, rBase;
	double height;
};
SubtreeBounds improvedSubtreeLayout(const Graph& g, int rId, map<int, double>& relVertexPos, 
	double r, double hS, double vS)
{
    vector<int> n = g.neighbors(rId);
	
	// Base case -- if we have no children, our bounding box is just a square
    if (n.size() == 0) return { r, r, r, r, 2 * r };

	vector<SubtreeBounds> cBnds;
	vector<double> cXpos;

	// The first thing to do is to recursively compute bounding boxes for each of our children, and
	// determine the x-coordinates of their root vertices.
	double minBase = Infinity; double maxBase = -Infinity; 
	double maxHeight = -Infinity;
    for (int i = 0; i < n.size(); ++i)
    {   
        int child = n[i];

		// Recursively compute the bounding box for child
        SubtreeBounds currBnd = improvedSubtreeLayout(g, child, relVertexPos, r, hS, vS);
		
		// currX is the x-coordinate of the child's root, relative to the left-most child, which is
		// positioned at 0.  We will eventually correct for this to ensure that all x-coordinates
		// are relative to the root of the current tree (that is, rId).
		double currX = 0;

		// If the current child has degree 0, or if the previously-placed child has degree 0,
		// ignore the width info from the bounding box.  This could potentially cause some overlaps,
		// but in practice it condenses the trees quite a bit, and any potential overlap will
		// generally be quite small
		if ((g.degree(child) == 0 && cXpos.size() > 0) || (i != 0 && g.degree(n[i-1]) == 0))
		{
			currX = cXpos.back() + 2 * r + hS;
		}

		// Otherwise, we position according to the computed bounding boxes
		else
		{	
			// If this is the first child, it just gets positioned at 0 (the following loop is never
			// entered).  Otherwise, we compute the minimum non-overlapping distance for each
			// previously-placed child, and set our current x-position to be the maximum of all
			// these minima (this way, we ensure no overlaps)
			for (int j = 0; j < cBnds.size(); ++j)
			{
				double xpos = cXpos[j];

				// Need to do two different computations depending on whether the current child's
				// height is taller than the other child.  The general idea here is that we find the
				// position on line segment 6 of the other child's trapezoid corresponding to the
				// left-most corner of our trapezoid (if we're shorter than our sibling), or the
				// position on line segment 7 of our trapezoid corresponding to the right-most
				// corner of the other trapezoid (if we're taller than our sibling).  Then we add in
				// the hSpace value to ensure some separation.
				if (cBnds[j].height < currBnd.height)
				{
					xpos += cBnds[j].rBase + hS + (cBnds[j].height / currBnd.height) * 
							(currBnd.lBase - currBnd.lTop) + currBnd.lTop;
				}
				else
				{
					xpos += cBnds[j].rTop + (currBnd.height / cBnds[j].height) *
							(cBnds[j].rBase - cBnds[j].rTop) + hS + currBnd.lBase;
				}

				// If the computed xpos is bigger than our current maximum, increase the current max
				if (xpos > currX) currX = xpos;
			}
		}
		// Store this child's bounding box
		cBnds.push_back(currBnd);
		cXpos.push_back(currX);

		// Compute the left and right base segments of our bounding box
		if (currX - currBnd.lBase < minBase) minBase = currX - currBnd.lBase;
		if (currX + currBnd.rBase > maxBase) maxBase = currX + currBnd.rBase;
		if (currBnd.height > maxHeight) maxHeight = currBnd.height;

        relVertexPos[child] = currX;
    }   

	// Now we need to adjust our coordinates so everything is centered at the root of the current
	// subtree.  We start with a top base length of 2r (i.e., just enough to contain the root), but
	// we'll expand this later if necessary.  The height of our bounding box is the height of our
	// largest child, plus the vertical spacing, plus the diameter of the root node
	double rootXAdj = (cXpos.back() - cXpos[0]) / 2;
	double lBase = rootXAdj - minBase; double rBase = maxBase - rootXAdj;
	double lTop = r; double rTop = r;
	double height = maxHeight + 2 * r + vS;

	// Go back through all the children and adjust their coordinate frames.  We also determine
	// whether the top line segment of the trapezoid needs to be expanded or not.
	for (int i = 0; i < n.size(); ++i)
	{
        relVertexPos[n[i]] -= rootXAdj;

		// We compute the positions of the left and right endpoints of the baseline and 
		// topline for each of the child bounding boxes
		double cTopXL = relVertexPos[n[i]] - cBnds[i].lTop;
		double cBaseXL = relVertexPos[n[i]] - cBnds[i].lBase;
		double cTopXR = relVertexPos[n[i]] + cBnds[i].rTop;
		double cBaseXR = relVertexPos[n[i]] + cBnds[i].rBase;
		double cTopHeight = 2 * r + vS;
		double cBaseHeight = cBnds[i].height + cTopHeight;

		// Here we compute the formula for line segments 6 and 7 on the trapezoid.  To determine if
		// the top line segment needs to be lengthened, we look at each child and see if it's
		// outside the current bounding box.  If it is, then we increase the length of either
		// segment 1 or 2, depending on which side of the box the child is outside
		double slopeL = height / (lBase - lTop);
		double yintL = slopeL * lTop;
		double slopeR = height / (rTop - rBase);
		double yintR = -slopeR * rTop;

		// Each of the following checks merely see if a point (x',y') corresponding to an extreme
		// point of the child trapezoid lies inside the currently-computed bounding box.
		// Specifically, if y = mx + b is the formula for line segment 6 (or 7), we only need to
		// perform adjustments if y' > mx' + b, which indicates that (x',y') is outside the box.
		//
		// In each case, there's a possibility that our slope is infinite, that is, the top of the
		// box needs to be adjusted to be even with the base of the box.  Otherwise, we just adjust
		// the line segment 6 (or 7) by shifting the top over to contain the point (x', y')
		//
		// We skip the check for the top segments of the child's bounding box if the child has no
		// descendents, because otherwise it would always force square bounding boxes, not what is
		// desired.

		// Check the bottom of the child (left)
		if (-Tolerance > slopeL * cBaseXL + yintL + cBaseHeight)
		{
			if (lBase + cBaseXL > Tolerance)
			{
				slopeL = (height - cBaseHeight) / (lBase + cBaseXL);
				yintL = -cBaseHeight - slopeL * cBaseXL;
				lTop = yintL / slopeL;
			}
			else lTop = lBase;
		}
		// Check the top of the child (left)
		if (g.degree(n[i]) > 0 && -Tolerance > slopeL * cTopXL + yintL + cTopHeight)
		{
			if (lBase + cTopXL > Tolerance)
			{
				slopeL = (height - cTopHeight) / (lBase + cTopXL);
				yintL = -cTopHeight - slopeL * cTopXL;
				lTop = yintL / slopeL;
			}
			else lTop = lBase;
		}
		// Check the bottom of the child (right)
		if (-Tolerance > slopeR * cBaseXR + yintR + cBaseHeight)
		{
			if (cBaseXR - rBase > Tolerance)
			{
				slopeR = (height - cBaseHeight) / (cBaseXR - rBase);
				yintR = -cBaseHeight - slopeR * cBaseXR;
				rTop = yintR / slopeR;
			}
			else rTop = rBase;
		}
		// Check the top of the child (right)
		if (g.degree(n[i]) > 0 && -Tolerance > slopeR * cTopXR + yintR + cTopHeight)
		{
			if (cTopXR - rBase > Tolerance)
			{
				slopeR = (height - cTopHeight) / (cTopXR - rBase);
				yintR = -cTopHeight - slopeR * cTopXR;
				rTop = yintR / slopeR;
			}
			else rTop = rBase;
		}

	}

	// Return the computed bounding box
    return { lTop, rTop, lBase, rBase, height};
}

GraphLayout improvedLayoutTreeLevel(const Graph& g, int radius, int hSpace, int vSpace)
{
    // Initialize a map of vertex positions relative to the vertex's parent; i.e.,
    // relVertexPos[i].x is the position of vertex i relative to the parent of i
    map<int, double> relVertexPos;

    // Compute the subtree layout in terms of relative vertex positions
    improvedSubtreeLayout(g, 0, relVertexPos, radius, hSpace, vSpace);

    // Translate relative positions into absolute positions using breadth-first search
    list<int> queue{0};
    GraphLayout layout;
    layout[0].first = 0;
    layout[0].second = 0;
    while (!queue.empty())
    {
        int currNode = queue.front(); queue.pop_front();
        vector<int> n = g.neighbors(currNode);
        for (int i = 0; i < n.size(); ++i)
        {
            int child = n[i];
            layout[child].first = layout[currNode].first + relVertexPos[child];
            layout[child].second = layout[currNode].second + vSpace + 2 * radius;
            queue.push_back(child);
        }
    }

    return layout;
}

}; // namespace graph
