# Convex-Hull Generator

This program was developed as a part of assignment for the course Algorithm Design and Analysis (CS60007)

Checkout the problem statement [here](algo-a2-ques.pdf)

## What is convex-hull?

Convex hull is the smallest convex polygon that contains all the points in a given set of points. It is also known as the convex envelope, convex closure, convex set, or convex figure. The convex hull may be visualized as the shape enclosed by a rubber band stretched around a finite set of points in the plane. For more information, checkout [this](https://en.wikipedia.org/wiki/Convex_hull)

## How to run the program?

1. Clone the repository
2. Run the following command in the terminal

```bash
g++ convex-hull.cpp -o p
```

3. Run the executable file

```bash
./p <input_file>
```

4. Three different svg files will be generated for each of the three cases (check readme.md for example)
5. Open the svg files in browser to see the output

## Input format:

```bash
n
g
x1 y1
x2 y2
.
.
.
xn yn
```

where n is the number of points, g is the origin point (bottom-left corner of the bounding box is at (g, g)
after the translation) and (xi, yi) are the coordinates of the ith point.

## Example

Sample input file can be found [here](aero.txt)

Sample output files:

In output, three SVG files are generated. The first one is the input points, the second one is the polygon generated from input points, and the third one is the convex hull with the input points.

1. [Points with bounding box](aero_1.svg)

![aero_1](aero_1.png)

2. [Polygon from given points](aero_2.svg)

![aero_2](aero_2.png)


3. [Convex hull](aero_3.svg)

![aero_3](aero_3.png)