#include "classVertex.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

//==============================================================================
// Class declaration for the Vertex class
//==============================================================================

// The declaration is in classVertex.h.  It's been #included in Line 1 above!

//==============================================================================
// Class implementation for the Vertex class
//==============================================================================

// This function is the constructor.  Based on the three inputs, it sets x, y,
// and z.  It also computes the corresponding distFromOrigin.

Vertex::Vertex(float xcoord, float ycoord, float zcoord)
{
    x = xcoord;
    y = ycoord;
    z = zcoord;
    distFromOrigin = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

//------------------------------------------------------------------------------

// This getter (provided for you) prints the values of the member variables.
void Vertex::show()
{
    cout << "(" << setw(8) << x << ", "
                << setw(8) << y << ", "
                << setw(8) << z << "), distFromOrigin = "
                << setw(8) << distFromOrigin << endl;
    return;
}

//------------------------------------------------------------------------------

// This operator function computes the sum of this Vertex and the input Vertex.

Vertex Vertex::operator+(Vertex input_vertex)
{

    return Vertex(x + input_vertex.x, y + input_vertex.y, z + input_vertex.z);
}

//------------------------------------------------------------------------------

// This operator function computes this Vertex minus the input Vertex.

Vertex Vertex::operator-(Vertex input_vertex)
{
    return Vertex(x - input_vertex.x, y - input_vertex.y, z - input_vertex.z);
}

//------------------------------------------------------------------------------

// This operator function computes the product of this Vertex and the input 
// scalar.

Vertex Vertex::operator*(float input_scalar)
{
    return Vertex(input_scalar*x, input_scalar*y, input_scalar*z);
}

//------------------------------------------------------------------------------

// This operator function computes this Vertex divided by the input scalar.

Vertex Vertex::operator/(float input_scalar)
{
    return Vertex(x/input_scalar, y/input_scalar, z/input_scalar);
}

//------------------------------------------------------------------------------

// This operator function computes the dot product of this Vertex and the input
// Vertex.

float Vertex::operator*(Vertex input_vertex)
{
    float dot_product = (x*input_vertex.x) + (y*input_vertex.y) + (z*input_vertex.z);
    return dot_product;
}

//------------------------------------------------------------------------------

// This operator function computes the cross product of this Vertex and the
// input Vertex.

Vertex Vertex::operator%(Vertex input_vertex)
{
    return Vertex(y*input_vertex.z - z*input_vertex.y,
                  z*input_vertex.x - x*input_vertex.z,
                  x*input_vertex.y - y*input_vertex.x);
}

//------------------------------------------------------------------------------

// This function computes the unit vector pointing from the origin toward this 
// Vertex.

Vertex Vertex::unit()
{
    return Vertex (x / distFromOrigin, y / distFromOrigin, z / distFromOrigin);
}

//------------------------------------------------------------------------------

// This function computes the projection of this Vertex using a given z0 
// (z-coordinate of the projection plane) and a given COP (center of projection).

Vertex Vertex::project(float z0, Vertex cop)
{
    float t = (z0 - cop.z) / (z - cop.z);
    return Vertex(cop.x + t*(x - cop.x), cop.y + t*(y - cop.y), z0);
}

//------------------------------------------------------------------------------

// This function rotates this Vertex by the input angle about the x-axis.

Vertex Vertex::rotx(float angleInRadians)
{
    return Vertex(x,
                  y*cos(angleInRadians) - z*sin(angleInRadians),
                  y*sin(angleInRadians) + z*cos(angleInRadians));
}

//------------------------------------------------------------------------------

// This function rotates this Vertex by the input angle about the y-axis.

Vertex Vertex::roty(float angleInRadians)
{
    return Vertex(x*cos(angleInRadians) + z*sin(angleInRadians),
                  y, -x*sin(angleInRadians) + z*cos(angleInRadians));
}

//------------------------------------------------------------------------------

// This function rotates this Vertex by the input angle about the z-axis.

Vertex Vertex::rotz(float angleInRadians)
{
    return Vertex(x*cos(angleInRadians) - y*sin(angleInRadians),
                  x*sin(angleInRadians) + y*cos(angleInRadians), z);
}


