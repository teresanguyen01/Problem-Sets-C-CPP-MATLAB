#include "moregraphics.h"
#include "classUniverse.h"
#include <iostream>
#include <fstream>
#include <cstdlib> // needed for exit(1)

using namespace std;

//==============================================================================
// Class declaration for the Universe class
//==============================================================================

// The declaration is in classUniverse.h.  It's been #included in Line 2 above!

//==============================================================================
// Class implementation for the Universe class
//
// NOTE:  Part of the class implementation is in prob17_provided_fcns.cpp.
//        You need to write the remaining functions by filling in the starter
//        code given below.
//==============================================================================

// This function is a default constructor.
// It sets the width, height, and wrap parameter to 0.  Sets array to nullptr.
// [Relevant sample code(s): classDataset_with_copyOfMyData_V3.cpp]
Universe::Universe()
{
    width = 0;
    height = 0;
    wrap = 0;
    array = nullptr;
}

//------------------------------------------------------------------------------

// This function is a constructor.
// It sets the values of the member variables.  Dynamically allocates memory
// for a Universe of a given width (1st arg) and height (2nd arg).  Sets all
// cells (i.e., all array entries) to 0.  The wrap setting is the 3rd arg.
// [Relevant sample code(s): classDataset_with_copyOfMyData_V3.cpp]
Universe::Universe(int widthInput, int heightInput, int wrapInput)
{
    // Set the values of the first three member variables.

    width = widthInput;
    height = heightInput;
    wrap = wrapInput;

    // Allocate memory for array that will store the cells.

    array = new int[width*height];

    // Set all cells in the array to 0.

    for (int i = 0; i < (width*height); i++)
    {
        array[i] = 0;
    }

}

//------------------------------------------------------------------------------

// This function is the destructor.
// It's called automatically when the object goes out of scope.
// It deallocates the memory associated with array.
// [Relevant sample code(s): classDataset_with_copyOfMyData_V3.cpp]
Universe::~Universe()
{
    // frees the memory associated with the array and rests the pointer
    if (array != nullptr)
    {
        delete[] array;
        array = nullptr;
    }
}

//------------------------------------------------------------------------------

// This operator function writes a Universe to a file.  The 1st, 2nd, and 3rd
// numbers in the file are width, height, and wrap setting.  After that are
// 0s/1s in a 2D integer array representing dead/live cells.
// [Relevant sample code(s): avgmaxFile_cppVersion.cpp]
void Universe::operator>>(char* filename)
{
    // Declarations

    // declare the output file
    ofstream outputFile;

    // Open the output file and make sure it opened properly.

    outputFile.open(filename);
    if (outputFile.fail())
    {
        cout << "Couldn't open file myoutput.dat.\n";
        return;
    }

    // Write the width, height, and wrap parameter.

    cout << width << " " << height << " " << wrap << endl;

    // Now write the 0s and 1s.

    // goes through each of the values in the array and writes the 0s and 1s
    // from the array to the output file
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // adds a space between each entry in the array
            outputFile << array[row * width + col] << " ";
        }
        outputFile << endl;
    }

    // Close the output file, since we're done writing to it.

    outputFile.close();

    return;

}

//------------------------------------------------------------------------------

// This operator function calculates the new generation by applying the four
// "genetic laws."  Then it displays the new generation and pauses briefly.
// The genetic laws (rules) are as follows.
// Survival:              Any live cell with 2 or 3 live neighbors survives.
// Death by isolation:    Any live cell with fewer than 2 live neighbors dies,
// Death by overcrowding: Any live cell with more than 3 live neighbors dies.
// Birth:                 Any dead cell with exactly 3 live neighbors is born
//                        (i.e., becomes a live cell in the next generation).
// [Relevant sample code(s): None, but look at the start of Lecture 11 to
//                           refresh your memory on the most efficient way
//                           to step through a 2D array.]
void Universe::operator<<(Universe& onTheRight)
{

    // loops through each of the values in the array
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // calculates the amount of neighbors that are alive for the
            // genetic laws
            int liveNeighbors = onTheRight.neighbors(row, col);
            int nextState = 0;

            // if equal to 1 --> handle the rules of any live cell
            if (onTheRight.getvalue(row, col))
            {
                // any live cell with 2 or 3 live neighbors survives
                if (liveNeighbors == 2 || liveNeighbors == 3)
                {
                    nextState = 1;
                }
                else if (liveNeighbors < 2) // any live with fewer than 2 live --> die
                {
                    nextState = 0;
                }
                else if (liveNeighbors > 3) // any live with more than 3 --> die
                {
                    nextState = 0;
                }
            }
            else
            {
                if (liveNeighbors == 3) // exactly 3 alive --> live
                {
                    nextState = 1;
                }
                else
                {
                    nextState = 0;
                }
            }

            // sets the value at row and col to the nextState
            setvalue(row, col, nextState);
        }
    }

    // Display the live cells in the graphics window.
    display();

    return;

}

//------------------------------------------------------------------------------

// This function returns the number of live neighbors of the cell at row,col.
int Universe::neighbors(int row, int col)
{
    // Declarations and initializations
    int numLiveNeigh = 0;

    if (!wrap) // "no wrap" case
    {
        // check for the neighbors (we know that the neighbors are 1 unit away (left, right, up, down)
        for (int i = row - 1; i <= row + 1; i++)
        {
            for (int j = col - 1; j <= col + 1; j++)
            {
                // skip if we are on our own cell (not a neighbor)
                if (i == row && j == col)
                {
                    continue;
                }

                // make sure that we are within the boundaries for wrap
                if (i >= 0 && i < height && j >= 0 && j < width)
                {
                    if (getvalue(i, j) == 1)
                    {
                        numLiveNeigh++;
                    }
                }
            }
        }
    }
    else // "wrap" case
    {
        // check for the neighbors (remember that neighbors are only 1 unit away
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                // skip if we are on our own cell (not a neighbor)
                if (i == 0 && j == 0)
                {
                    continue;
                }

                // finds the new row and column value based on wrapping
                int nRow = (row + i + height) % height;
                int nCol = (col + j + width) % width;

                // if the cell is alive, add 1.
                if (getvalue(nRow, nCol) == 1)
                {
                    numLiveNeigh++;
                }
            }
        }
    }

    return numLiveNeigh;
}
