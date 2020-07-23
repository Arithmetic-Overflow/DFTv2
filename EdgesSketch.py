#Edge Detection Algorithm

from PIL import Image, ImageColor
from math import floor
import csv
import os
import random
import pygame

# euclidian distance in 2D space
# (not the same as weighted euclidian distance)
def dist(p1, p2):
        return ((p1[0]-p2[0])**2 + (p1[1]-p2[1])**2)**0.5
    
    
def waitForEscape():
    pygame.event.pump()
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                return True
    else:
        return False
    
    
def waitForSpace():
    pygame.event.pump()
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                return True
    else:
        return False


# puts the coordinates of every edge in a program into a csv
def findEdges(filename, threshold = 10, downscale = 1):

    # weights for a weighted euclidian distance formula
    # this is used to find the distance between colours
    rWeight = 2
    gWeight = 4
    bWeight = 3

    # finds the width/height of the image (with downscaling)
    im = Image.open(os.path.join("Images", filename))
    px = im.load()
    im.close()
    w, h = im.size

    w //= downscale
    h //= downscale
    print("Width: {}\nHeight: {}".format(w, h))

    edgeCount = 0

    # open an empty file to write everything in
    with open(os.path.join("ImageData", "Edges.csv"), mode = "w") as dp:
        dp.truncate()
        dpLog = csv.writer(dp, lineterminator = "\n", delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

        for i in range(0, w-1):
            for j in range(0, h-1):
                row = downscale*j
                column = downscale*i
                nextRow = downscale*(j+1)
                nextColumn = downscale*(i+1)

                # contrast with the cell below/to the right
                c0 = 0
                c1 = 0

                # calculate the 'contrast' as the colour distance
                c0 += rWeight*abs(px[column, row][0] - px[column, nextRow][0])
                c0 += gWeight*abs(px[column, row][1] - px[column, nextRow][1])
                c0 += bWeight*abs(px[column, row][2] - px[column, nextRow][2])

                c1 += rWeight*abs(px[column, row][0] - px[nextColumn, row][0])
                c1 += gWeight*abs(px[column, row][1] - px[nextColumn, row][1])
                c1 += bWeight*abs(px[column, row][2] - px[nextColumn, row][2])
                
                c0 = c0**0.5
                c1 = c1**0.5

                # the contrast is the biggest colour distance
                contrast = max(c0, c1)
                
                # only consider the point an 'edge' if the colour distance is high (bigger than threshold)
                if contrast > threshold:
                    dpLog.writerow([i,j])
                    edgeCount += 1

    print("\n{} Edges Found In The Program".format(edgeCount))
    

# plots every edge coordinate
def plot(scale = 1, RGB = (255, 255, 255), backColour = (0, 0, 0)):
    pygame.init()

    # set the pygame window to be size of the current monitor display
    windowInfo = pygame.display.Info()
    canvasWidth = windowInfo.current_w
    canvasHeight = windowInfo.current_h
    canvas = pygame.display.set_mode((canvasWidth, canvasHeight))#, flags = pygame.FULLSCREEN)
    canvas.fill(backColour)

    # load all the edges and turn them into a list
    file = open(os.path.join("ImageData", "Edges.csv"), "r")
    fread = file.read()
    file.close()
    fread = fread.replace("\n", ",")
    fread = fread.split(",")

    # draw each point as a line with the same start/end
    for i in range(0, len(fread)-1, 2):
        x = int(scale*int(fread[i]))
        y = int(scale*int(fread[i+1]))

        pygame.draw.line(canvas, RGB, (x, y),(x, y), 1)

    pygame.display.update()

    while True:
        if waitForEscape():
            pygame.quit()
            return
    

# draws the path instantly
def instantPath(canvas, scale, coordinates, threshold, RGB):
    
    for i in range(len(coordinates)-1):
        x0 = int(scale*int(coordinates[i][0]))
        y0 = int(scale*int(coordinates[i][1]))
        x1 = int(scale*int(coordinates[i+1][0]))
        y1 = int(scale*int(coordinates[i+1][1]))
        
        if dist((x0,y0),(x1,y1)) < threshold:
            pygame.draw.line(canvas, RGB, (x0, y0),(x1, y1), 1)
            
        pygame.event.pump()

        if waitForEscape():
            pygame.quit()
            return
                        
    pygame.display.update()

    while True:
        if waitForEscape():
            pygame.quit()
            return


# draws the path and updates for every line drawn
def proceduralPath(canvas, scale, coordinates, threshold, RGB):
    
    pygame.display.update()
    
    while True:
        if waitForSpace():
            break
            
    
    for i in range(len(coordinates)-1):
        x0 = int(scale*int(coordinates[i][0]))
        y0 = int(scale*int(coordinates[i][1]))
        x1 = int(scale*int(coordinates[i+1][0]))
        y1 = int(scale*int(coordinates[i+1][1]))
        
        if dist((x0,y0),(x1,y1)) < threshold:
            pygame.draw.line(canvas, RGB, (x0, y0),(x1, y1), 1)
            
        pygame.event.pump()
        pygame.display.update()

        if waitForEscape():
            pygame.quit()
            print("Drawing Terminated")
            return

    while True:
        if waitForEscape():
            pygame.quit()
            return


# draws the pathing of an image by connecting close consecutive points
def drawPath(scale = 1, procedural = True, RGB = (0, 0, 0), backColour = (220, 200, 150)):
    
    pygame.init()

    file = open(os.path.join("ImageData", "Path.csv"), "r")
    fread = file.read()
    file.close()
    fread = fread.split("\n")
    
    coordinates = []
    for i in fread:
        coordinates.append(i.split(","))
    coordinates = coordinates[:-1]
    
    windowInfo = pygame.display.Info()
    canvasWidth = windowInfo.current_w
    canvasHeight = windowInfo.current_h
    canvas = pygame.display.set_mode((canvasWidth, canvasHeight), flags = pygame.FULLSCREEN)
    canvas.fill(backColour)
    
    threshold = 4

    if procedural:
        proceduralPath(canvas, scale, coordinates, threshold, RGB)
    else:
        instantPath(canvas, scale, coordinates, threshold, RGB)