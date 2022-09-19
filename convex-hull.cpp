/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STEPS TO RUN THE CODE
// 1. Compile the code using "g++ convex-hull.cpp -o p"
// 2. Run the code using "./p aero.txt" in Ubuntu "p aero.txt" in Windows
// 3. Three different svg files will be generated for each of the three cases (check readme.md for example)
// 4. Open the svg files in browser to see the output
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <limits.h>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

// structure to store coordinate as a point
struct Point
{
    int x;
    int y;
};

// comparison function to compare points by x-coordinate
bool compare(Point &p1, Point &p2)
{
    return p1.x < p2.x;
}

// function to print the points
void printPoints(struct Point arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << arr[i].x << "," << arr[i].y << endl;
    }
}

// Checks whether the point is above the line or below the line
int pointLocator(struct Point center, struct Point p, struct Point q)
{
    // cross product of vector center->p and center->q
    int res = (p.x - center.x) * (q.y - center.y) - (q.x - center.x) * (p.y - center.y);

    // colinear
    if (res == 0)
        return 0;
    // if positive then q is above
    if (res > 0)
        return 1;
    // if negative then q is below
    return -1;
}

// Brute force method to find the convex hull
// arr: array of points
// n: number of points
// start: starting index of the array
// end: ending index of the array
// count: reference to count variable to keep total number of points in convex hull
struct Point *convexHull(struct Point arr[], int start, int end, int *count)
{
    // n: number of points
    int n = end - start + 1;
    struct Point *hull = new struct Point[n];
    int k = 0;
    // locating the next point in the convex hull
    for (int i = start; i <= end; i++)
    {
        while (k >= 2 && pointLocator(hull[k - 2], hull[k - 1], arr[i]) <= 0)
            k--;
        // adding the point to the convex hull
        hull[k++] = arr[i];
    }
    // locating the next point in the convex hull
    for (int i = end - 1, t = k + 1; i >= start; i--)
    {
        while (k >= t && pointLocator(hull[k - 2], hull[k - 1], arr[i]) <= 0)
            k--;
        // adding the point to the convex hull
        hull[k++] = arr[i];
    }
    *count = k - 1;
    // returning the convex hull
    return hull;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Part-1 (Transforming the points and drawing the boundary rectangle and points)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TransformPoints(string filename_prefix, int n, int g, struct Point pArray[], int maxX, int minX, int newMaxY, int newminY)
{
    FILE *svgWrtr;
    // filename for first program output
    string p1_op_filename = filename_prefix + "_1.svg";
    // Open output file for part1
    svgWrtr = fopen(p1_op_filename.c_str(), "w"); // c_str(): converts string to char array

    // writing SVG header
    fprintf(svgWrtr, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    // writing canvas
    fprintf(svgWrtr, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g);
    // drawing boundary rect
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:rgb(0,0,0)\" />\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g, 0, 0);
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:red;stroke-dasharray:2\" />\n", maxX - minX, newMaxY - newminY, g, g);
    // plotting points
    for (int i = 0; i < n; i++)
    {
        fprintf(svgWrtr, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />\n", pArray[i].x + g, pArray[i].y + g, 1);
    }
    // closing svgs
    fprintf(svgWrtr, "</svg>");
    fclose(svgWrtr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Part-2 (Drawing the polygon and points)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string DrawPolygonNPoints(string filename_prefix, int n, int g, struct Point pArray[], int maxX, int minX, int newMaxY, int newminY)
{
    int count = n;
    // sorting points in increasing order of x
    sort(pArray, pArray + n, compare);
    // Arrange the points in such a way that such that for any point p, there are exactly two points in pArray having distance g or √2g from p.
    struct Point curPoint = pArray[0];

    // Array to keep status of visited points
    bool visited[n];
    // Initialize visited array with false;
    for (int i = 0; i < n; i++)
        visited[i] = false;

    FILE *svgWrtr;
    // filename for part2
    string p2_op_filename = filename_prefix + "_2.svg";
    // open output file for part2
    svgWrtr = fopen(p2_op_filename.c_str(), "w");
    // writing SVG header
    fprintf(svgWrtr, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    // writing canvas
    fprintf(svgWrtr, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g);
    // drawing boundary rect
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:rgb(0,0,0)\" />\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g, 0, 0);
    // Taking leftmost point to start tracing vertices in clockwise direction
    visited[0] = true;
    // dots: string to store points as circle elements
    string dots = "";
    // storing leftmost point in dots
    dots += "<circle cx=\"" + to_string(pArray[0].x + g) + "\" cy=\"" + to_string(pArray[0].y + g) + "\" r=\"1\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />\n";
    // points: string to store coordinates for polygon
    string points = "<polygon points=\"";
    points = points + to_string(pArray[0].x + g) + "," + to_string(pArray[0].y + g) + " ";

    // Tracing the vertices in clockwise direction
    while (count--)
    {
        // Find the next point of curPoint
        for (int i = 1; i < n; i++)
        {
            if (visited[i] == false)
            {
                // if distance between curPoint and pArray[i] is equal to g or √2g then make pArray[i] as next point
                if (sqrt(pow(pArray[i].x - curPoint.x, 2) + pow(pArray[i].y - curPoint.y, 2)) == g || sqrt(pow(pArray[i].x - curPoint.x, 2) + pow(pArray[i].y - curPoint.y, 2)) == sqrt(2) * g)
                {
                    // marking current point as visited so that we don't visit it again
                    visited[i] = true;
                    // updating current point
                    curPoint = pArray[i];
                    // appending dots as circle
                    dots += "<circle cx=\"" + to_string(pArray[i].x + g) + "\" cy=\"" + to_string(pArray[i].y + g) + "\" r=\"1\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />\n";
                    // appending coordinates for polygon
                    points = points + to_string(pArray[i].x + g) + "," + to_string(pArray[i].y + g) + " ";
                    break;
                }
            }
        }
    }
    points += "\" fill=\"yellow\" stroke=\"black\" fillRule= 'nonzero' />\n";
    // write polygon to file
    fprintf(svgWrtr, "%s", points.c_str());
    // write points to file
    fprintf(svgWrtr, "%s", dots.c_str());
    // close svg tag
    fprintf(svgWrtr, "</svg>");
    fclose(svgWrtr);
    return points;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Part-3 (Drawing the convex hull)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GenerateConvexHull(string filename_prefix, int n, int g, struct Point pArray[], int maxX, int minX, int newMaxY, int newminY, string points)
{
    FILE *svgWrtr;
    // filename for part3
    string p3_op_filename = filename_prefix + "_3.svg";
    // open output file for part3
    svgWrtr = fopen(p3_op_filename.c_str(), "w");
    // writing SVG header
    fprintf(svgWrtr, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    // writing canvas
    fprintf(svgWrtr, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g);
    // drawing boundary rect
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:rgb(0,0,0)\" />\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g, 0, 0);
    // program to find convex hull using divide and conquer
    int count = 0;
    sort(pArray, pArray + n, compare);
    struct Point *cvh = convexHull(pArray, 0, n - 1, &count);
    // print hull points
    printf("Convex Hull points are:\n");
    for (int i = 0; i < count; i++)
    {
        printf("(%d, %d)\n", cvh[i].x, cvh[i].y);
    }
    string hullPoints = "";
    string hullPolygon = "";
    // drawing convex hull
    for (int i = 0; i < count; i++)
    {
        // circle to show hull vertices
        hullPoints += "<circle cx=\"" + to_string(cvh[i].x + g) + "\" cy=\"" + to_string(cvh[i].y + g) + "\" r=\"1\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />\n";
        // appending hull points for drawing hull polygon
        hullPolygon += to_string(cvh[i].x + g) + "," + to_string(cvh[i].y + g) + " ";
    }
    hullPolygon = "<polygon points=\"" + hullPolygon + "\" fill=\"cyan\" stroke=\"black\" fillRule= 'nonzero' />\n";
    // drawing polygon
    fprintf(svgWrtr, "%s", hullPolygon.c_str());
    fprintf(svgWrtr, "%s", hullPoints.c_str());
    fprintf(svgWrtr, "%s", points.c_str());
    fprintf(svgWrtr, "</svg>");
    fclose(svgWrtr);
}

int main(int argc, char *argv[])
{
    cout << argv[1] << endl;
    // pointer to read file
    FILE *fptr;
    // pointer to write output to file
    FILE *svgWrtr;
    // Open input file
    fptr = fopen(argv[1], "r");
    if (fptr == NULL)
    {
        cout << "Can not open the file";
        return 0;
    }

    int g;
    int n;
    // Read n and g
    fscanf(fptr, "%d", &g);
    fscanf(fptr, "%d", &n);

    cout << "g:" << g << " n:" << n << endl;

    // Temporary variables to read x and y
    int x, y;
    // Array of points
    struct Point pArray[n];

    int maxY = 0;
    int minY = 0;
    int minX = INT_MAX;
    int maxX = INT_MIN;
    // Scan the points from file
    for (int i = 0; i < n; i++)
    {
        fscanf(fptr, "%d %d", &x, &y);
        // Find the max and min values of x and y
        if (y > maxY)
        {
            maxY = y;
        }
        if (y < minY)
        {
            minY = y;
        }
        if (x < minX)
        {
            minX = x;
        }
        if (x > maxX)
        {
            maxX = x;
        }
        pArray[i].x = x;
        pArray[i].y = y;
    }
    // new minimum and maximum y values after adding g
    int newMaxY = 0;
    int newminY = INT_MAX;
    // new minimum and maximum x values and their indices
    int minXIndex = 0;
    int minXValue = INT_MAX;
    int maxXIndex = 0;
    int maxXValue = INT_MIN;

    // Tranforming the SVG coords
    // Translating the points by rotating them by 180 degrees
    // and then translating them by -minX (Basically normalizing the points)
    for (int i = 0; i < n; i++)
    {
        // subtracting maxY from all y value
        pArray[i].y = maxY - pArray[i].y;
        // Normalizing x coords to shift to origin
        pArray[i].x = pArray[i].x - minX;
        if (pArray[i].y > newMaxY)
        {
            newMaxY = pArray[i].y;
        }
        if (pArray[i].y < newminY)
        {
            newminY = pArray[i].y;
        }
    }

    // find minXIndex and maxXIndex
    for (int i = 0; i < n; i++)
    {
        if (pArray[i].x < minXValue)
        {
            minXValue = pArray[i].x;
            minXIndex = i;
        }
        if (pArray[i].x > maxXValue)
        {
            maxXValue = pArray[i].x;
            maxXIndex = i;
        }
    }

    // Width and Height will include the gap at the borders
    cout << "Canvas width:" << maxX - minX + 2 * g << endl;
    cout << "Canvas height:" << newMaxY - newminY + 2 * g << endl;
    // string to store filename without extension
    string filename_prefix = "";
    // remove .txt from argv[1] and store it in p1_op_filename
    for (int i = 0; i < strlen(argv[1]) - 4; i++)
    {
        filename_prefix += argv[1][i];
    }

    // Part-1
    TransformPoints(filename_prefix, n, g, pArray, maxX, minX, newMaxY, newminY);
    // Part-2
    // points: string which stores points to be drawn (This will be used in part-3 also)
    string points = DrawPolygonNPoints(filename_prefix, n, g, pArray, maxX, minX, newMaxY, newminY);
    // Part-3
    GenerateConvexHull(filename_prefix, n, g, pArray, maxX, minX, newMaxY, newminY, points);
}