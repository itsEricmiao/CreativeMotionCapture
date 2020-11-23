import dlib
from imutils import face_utils
from os import listdir
from os.path import isfile, join
import numpy as np
import cv2

def findFiles(path):
    newfiles = list()
    onlyfiles = [f for f in listdir(path) if isfile(join(path, f))]
    for each in onlyfiles:
        if each.endswith(".jpg"):
            newfiles.append(each)
    return newfiles

# Read points from text file
def readPoints(path) :
    # Create an array of points.
    points = [];
    # Read points
    with open(path) as file :
        for line in file :
            x, y = line.split()
            points.append((int(x), int(y)))
    return points


detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('lib/shape_predictor_68_face_landmarks.dat')

def detect_face(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # detect faces in the grayscale image
    rects = detector(gray, 1)
    return gray, rects

def landmark_detection(gray, rects):
    if len(rects) >= 2:
        points1 = face_utils.shape_to_np(predictor(gray, rects[0])).tolist()
        points2 = face_utils.shape_to_np(predictor(gray, rects[1])).tolist()

        points1 = [(int(i[0]), int(i[1])) for i in points1]
        points2 = [(int(i[0]), int(i[1])) for i in points2]
        return points1, points2
    elif len(rects) == 1:
        points1 = face_utils.shape_to_np(predictor(gray, rects[0])).astype(np.int32).tolist()
        points1 = [(int(i[0]), int(i[1])) for i in points1]
        return points1, points1
    else:
        return None


