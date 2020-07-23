# DFTv2
The same DFT algorithm as before but with all computations written in C++ instead of python for efficiency concerns.
The DFT algorithm inputs an image (.jpg) and outputs a drawing of the image using epicycles.

All graphing is done using the pygame library and all the image processing is done using pillow (in python).
Currently the computations in order to draw the image using epicycles are extremely expensive so it takes a considerable amount of time to draw anything with >5000 points.
This means that the drawing functionality using epicycles is limited to about drawings with about 5,000 points. 
I am going to look into ways to streamline the drawing process in the future.
A possibly right now is to use numpy instead of standard python in order to do the calculations.

Usage of the program:
  All image processing/drawing is functions in DFTProcessing.py
  All computations are in PathEdgesAndTransform.cpp
  
  Run the findEdges() function using a filename (and optional parameters)
  Build and run "PathEdgesAndTransform" in order to compute the path and DFT
  You can use drawPath() to draw the path without the DFT
  Alternative sketchDFT() draws it through the DFT using epicycles
