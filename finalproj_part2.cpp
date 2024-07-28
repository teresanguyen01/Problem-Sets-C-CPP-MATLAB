#include <QApplication>
#include "classVertex.h"
#include "structPoly_classSolid.h"
#include "finalgraphics.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib> // needed for exit(1)

using namespace std;

#define MAXVERTSINPOLY 10
#define GRAPHICSWINDOWPIXELS 500
// When you call plotOn, use GRAPHICSWINDOWPIXELS twice in its argument list.

//==============================================================================
// Class declaration for the Solid class
//==============================================================================

// The declaration is in structPoly_classSolid.h.  It's been #included in Line 3
// above!

//==============================================================================
// Class implementation for the Solid class
//==============================================================================

// This function is the constructor.  It sets each member variable to zero or
// to nullptr (except cop, which is a Vertex object).
// [DON'T CHANGE THIS FUNCTION, which has been written for you.]
Solid::Solid()
{
    nVertsInObj = 0;
    vertArray = nullptr;
    nPolysInObj = 0;
    polyArray = nullptr;
    maxDistFromOrigin = 0.;
    z0 = 0.;
}

//------------------------------------------------------------------------------

// This function is the destructor.  It's called automatically when the object
// goes out of scope.  It deallocates memory associated with vertArray and
// polyArray.
// [DON'T CHANGE THIS FUNCTION, which has been written for you.]
Solid::~Solid()
{
    if (vertArray != nullptr)
    {
        delete[] vertArray;
        vertArray = nullptr;
    }
    if (polyArray != nullptr)
    {
        delete[] polyArray;
        polyArray = nullptr;
    }
}

//------------------------------------------------------------------------------

// This function reads in the geometric description of the solid object (i.e.,
// vertex list and polygon list) from the specified file, as well as the color
// of each polygon.  It also calculates the centroid of each polygon.  Finally,
// it determines maxDistFromOrigin, z0, and the COP.
void Solid::readInFromFile(char* filename)
{
    // Declarations
    float xFromFile, yFromFile, zFromFile;
    ifstream inputFile;

    // Open the input file and make sure it opened properly.
    inputFile.open(filename);
    if (inputFile.fail())
    {
        cout << "Couldn't open file " << filename << ".\n";
        exit(1); // stop execution of the code immediately
    }

    // Read in the number of vertices.

    inputFile >> nVertsInObj;

    // Allocate memory for array that will store the Vertex objects.

    vertArray = new Vertex[nVertsInObj];
    if (vertArray == nullptr)
    {
        cout << "\nCouldn't allocate memory for vertArray.\n";
        return;
    }

    // Read in the (x,y,z) data for the Vertex objects.

    for (int iVert=0; iVert < nVertsInObj; iVert++)
    {
        inputFile >> xFromFile >> yFromFile >> zFromFile;
        vertArray[iVert] = Vertex(xFromFile, yFromFile, zFromFile);
    }

    // Read in the number of number of polygons.

    inputFile >> nPolysInObj;

    // Allocate memory for the array that will store the Polygon structures.

    polyArray = new Poly[nPolysInObj];
    if (polyArray == nullptr)
    {
        cout << "\nCouldn't allocate memory for polyArray.\n";
        return;
    }

    // Read in the data for the poly structures (number of verticies in the polygon,
    // list of verticies in the polygon, color of the polygon, centroid of the polygon.

    // Takes each row and the first value and stores it in the structure's nVertsInThisPoly
    for (int row = 0; row < nPolysInObj; row++)
    {
        inputFile >> polyArray[row].nVertsInThisPoly;
        // We know that the next numbers in the row are the verticies in the poly.
        // We then store all the numbers into the structure of the list of verticies in the poly
        for (int col = 0; col < polyArray[row].nVertsInThisPoly; col++)
        {
            inputFile >> polyArray[row].listOfVertsInThisPoly[col];
        }
    }
    // Next, we store all the respective colors to each of the structures
    for (int indexCol = 0; indexCol < nPolysInObj; indexCol++)
    {
        inputFile >> polyArray[indexCol].colorOfThisPoly;
    }

    // Now we calculate the centroid of each of the polygons.
    // The centroid is calculated by adding all the x, y, z coordinates of the polygon
    // and dividing by how many verticies the polygon has.
    for (int indexPoly = 0; indexPoly < nPolysInObj; indexPoly++)
    {
        // create a centroid at (0,0,0) so that way we can add all the coordinates
        Vertex centroid(0,0,0);

        // goes through each value in the list of Verticies and adds them to the centroid
        for (int indexVer = 0; indexVer < polyArray[indexPoly].nVertsInThisPoly; indexVer++)
        {
            centroid = centroid + vertArray[polyArray[indexPoly].listOfVertsInThisPoly[indexVer]];
        }

        // We use our operator function to divide the centroid by the number of verticies in the polygon
        polyArray[indexPoly].centroidOfThisPoly = centroid / polyArray[indexPoly].nVertsInThisPoly;
    }

    // You need to figure out the steps for reading in the rest of the data and
    // doing any necessary calculations -- and you need to put appropriate
    // COMMENTS to describe what you're doing.  After that, as shown in the
    // comments below, there are only three things left to do, the second and
    // third of which have been done for you.

    // Determine the max distance from the origin (i.e., maxDistFromOrigin) of
    // any vertex in the solid object.

    // index through every vertex in the array and calculate the distFromOrigin using our constructor
    for (int indexVer = 0; indexVer < nVertsInObj; indexVer++)
    {
        // if the vertex has a higher distance from the origin than the max, set the max to the vertex's distance
        // from origin
        if (vertArray[indexVer].distFromOrigin > maxDistFromOrigin)
        {
            maxDistFromOrigin = vertArray[indexVer].distFromOrigin;
        }
    }

    // Ensure that the projection plane is behind the rotating object.
    z0 = -2.*maxDistFromOrigin;

    // The COP is on the positive z-axis, so its x- and y-coordinates are both 0.
    // Its z-coordinate is large to reduce distortion.
    cop = Vertex(0., 0., 50.*maxDistFromOrigin);

    return;
}

//------------------------------------------------------------------------------

// This function projects the vertices and plots the result.  The first time
// the function is called, it opens the graphics window.  Subsequent calls must
// NOT re-open the window, because it's already open.
void Solid::projectAndPlot()
{
    // Declarations and initializations
    static int firstTime=1;
    float xmin, xmax, ymin, ymax;
    float intensity;

    // Allocate memory for an array of Vertex objects, which will contain
    // the projections of the vertices in our Solid object.
    Vertex* projectedVertices = new Vertex[nVertsInObj];
    if (projectedVertices == nullptr)
    {
        cout << "Couldn't allocate memory for projectedVertices.\n";
        exit(1); // stop execution of the code immediately
    }

    // Now project ALL the vertices of our Solid object onto the projection plane,
    // and store in projectedVertices.  This way, we avoid having to recompute
    // the projections of the visible vertices later, given that they each belong
    // to multiple polygons.

    // use the project member function to project all verticies and store into
    // projected verticies
    for (int index = 0; index < nVertsInObj; index++)
    {
        projectedVertices[index] = vertArray[index].project(z0, cop);
    }


    // If this is the first time that the projectAndPlot function is being
    // called, the graphics window has not yet been initialized.  Open the
    // window and scale it.  [Note that we're using a static int to determine
    // whether the window is already open; if you've forgotten what a static
    // variable is, see Lecture 06, Slides 11-14.]
    if (firstTime)
    {
        // Open a square graphics window via a call to plotOn.

        plotOn(GRAPHICSWINDOWPIXELS, GRAPHICSWINDOWPIXELS);

        // In order to scale the axes of the graphics window, we need to know
        // the extents of the projected image in ANY ONE coordinate direction.
        // Let's consider a point located at (maxDistFromOrigin, 0, 0).  The
        // x-coordinate of the projection of this point is the largest value
        // that we would ever need to plot in the graphics window in ANY
        // coordinate direction when drawing our rotating object.
        Vertex furthest(maxDistFromOrigin, 0., 0.);
        Vertex projectedFurthest = furthest.project(z0, cop);
        xmax = projectedFurthest.x * 1.5;

        // Now increase xmax to allow for a "margin" of white space around the
        // rotating object.  Also set xmin, ymax, and ymin. (HELP HERE)

        xmin = -xmax;
        ymax = xmax;
        ymin = -ymax;

        // Finally, scale the axes of the graphics window.

        xScale(xmin, xmax);
        yScale(ymin, ymax);

        // We're done with tasks that must completed ONLY the first time this
        // function is called, so we reset firstTime.

        firstTime = 0;

    }

    // Now that the graphics window is guaranteed to be open, draw the projection
    // of our Solid object.

    // go through every polygon
    for (int indexPoly = 0; indexPoly < nPolysInObj; indexPoly++)
    {
        // access the current polygon
        Poly& poly = polyArray[indexPoly];

        // create an array of Vertex objects
        Vertex* polygonVerts = new Vertex[poly.nVertsInThisPoly];
        if (polygonVerts == nullptr)
        {
            cout << "Couldn't allocate memory for projectedVertices.\n";
            exit(1); // stop execution of the code immediately
        }

        // go through each vertex in the polygon
        for (int indexVert = 0; indexVert < poly.nVertsInThisPoly; indexVert++)
        {
            // copy the projected verticies for the current polygon from the overall projected
            // verticies
            polygonVerts[indexVert] = projectedVertices[poly.listOfVertsInThisPoly[indexVert]];
        }

        // calculate the unit normal vector for the intensity
        Vertex unitNormal = polyNorm(indexPoly);

        // calculate the centroid toward the cop by finding the vector and then turning it into a unit vector
        Vertex centroidTowardCOP = (cop - poly.centroidOfThisPoly).unit();

        // the intensity is calculated by the dot product
        intensity = unitNormal * centroidTowardCOP;

        // if the dot product of unitNormal and centroidTowardCOP is greater than 0
        // draw the polygon. Else don't print.
        if (intensity > 0)
        {
            drawPoly(polygonVerts, poly.nVertsInThisPoly, poly.colorOfThisPoly, intensity);
        }

        // deallocate the memory
        delete[] polygonVerts;
    }

    // deallocate the memory
    delete[] projectedVertices;

    return;

}

//------------------------------------------------------------------------------

// This function rotates all vertices in the object about the specified axis
// (X, Y, or Z) by the specified angle.    Also rotates the centroids of all
// polygons.
void Solid::rotate(int axis, float angleInRadians)
{
    // go through every vertex and if the axis is 0 (x), rotate about the x. 1 is y.
    // 2 is z.
    for (int index = 0; index < nVertsInObj; index++)
    {
        if (axis == 0)
        {
            vertArray[index] = vertArray[index].rotx(angleInRadians);
        }
        else if (axis == 1)
        {
            vertArray[index] = vertArray[index].roty(angleInRadians);
        }
        else if (axis == 2)
        {
            vertArray[index] = vertArray[index].rotz(angleInRadians);
        }
    }

    // go through the same process but this time for the centroid of the polygon
    for (int index = 0; index < nPolysInObj; index++)
    {
        if (axis == 0)
        {
            polyArray[index].centroidOfThisPoly = polyArray[index].centroidOfThisPoly.rotx(angleInRadians);
        }
        else if (axis == 1)
        {
            polyArray[index].centroidOfThisPoly = polyArray[index].centroidOfThisPoly.roty(angleInRadians);
        }
        else if (axis == 2)
        {
            polyArray[index].centroidOfThisPoly = polyArray[index].centroidOfThisPoly.rotz(angleInRadians);
        }
    }
    return;

}

//------------------------------------------------------------------------------

// This function computes the outward unit normal for polygon iPoly.
Vertex Solid::polyNorm(int iPoly)
{
    // Declarations and initializations

    Poly poly = polyArray[iPoly];
    Vertex V1;
    Vertex V2;
    Vertex unitNormal;

    // Copy the first three vertices in this polygon into variables having
    // shorter names (P0, P1, and P2), which will make them easier to refer to.

    Vertex P0 = vertArray[poly.listOfVertsInThisPoly[0]];
    Vertex P1 = vertArray[poly.listOfVertsInThisPoly[1]];
    Vertex P2 = vertArray[poly.listOfVertsInThisPoly[2]];

    // Do the cross product of the appropriate vectors and then normalize the
    // result so that it is a unit vector.

    V1 = P1 - P0;
    V2 = P2 - P0;

    // calculate the cross product and return the unit vector
    unitNormal = V1 % V2;
    return unitNormal.unit();

}
