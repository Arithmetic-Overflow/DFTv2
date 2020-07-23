from EdgesSketch import findEdges, drawPath, plot
from EpicycleSketch import sketchDFT

import random
import csv
import pygame
import os
from PIL import Image, ImageColor

# pygame.init()

# Functions:
#     findEdges(filename, threshold, downscale)

#     plot(scale, line_colour, background_colour)

#     drawPath(scale, procedural_path (False = instant drawing), RGB, background_colour)
#     press space to begin drawing if procedural_path is True

#     sketchDFT(lowpass, highpass) (both lowpass and highpass are %s)
#     lowpass distorts the shape, highpass lowers fidelity (and speeds up your drawing)