import cv2
import os
from os.path import isfile, join


def create_video(pathIn, pathOut, fps=1):
    """Create uncompressed video from images

    Args:
        pathIn (str): Path to image sequence folder
        pathOut (str): Path to video
        fps (int): Frames per second. Defaults to 1.
    """
    frame_array = []
    files = [f for f in os.listdir(pathIn) if isfile(join(pathIn, f))]
    #for sorting the file names properly
    files.sort(key = lambda x: x[:-4])
    files.sort()
    frame_array = []
    files = [f for f in os.listdir(pathIn) if isfile(join(pathIn, f))]
    #for sorting the file names properly
    files.sort(key = lambda x: x[:-4])
    for i in range(len(files)):
        filename=pathIn + files[i]
        #reading each files
        img = cv2.imread(filename)
        height, width, layers = img.shape
        size = (width,height)
        
        #inserting the frames into an image array
        frame_array.append(img)
    out = cv2.VideoWriter(pathOut,0, fps, size)
    for i in range(len(frame_array)):
        # writing to a image array
        out.write(frame_array[i])
    out.release()