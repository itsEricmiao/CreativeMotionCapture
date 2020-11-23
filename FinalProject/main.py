import cv2
from FaceSwap import swap_images
from Utilities import *

mypath = "funny"
def interface():
    print("Welcome to Hybrid Image Program")
    print("Below are a few images you can choose from: ")
    counter = 1;
    files = findFiles(mypath)
    for each in files:
        print(counter, " : ", each)
        counter = counter + 1
    val = int(input("Enter your value: "))
    if ((val) < 0 and (val) > counter):
        print("INVALID INPUT")
        interface()
    else:
        filename = str(mypath) + "/" + str(files[val-1])

    return filename

if __name__ == '__main__' :
    filename = interface()

    # Read images
    cap = cv2.VideoCapture(0)
    mypath = "data"
    while(True):
    # Capture frame-by-frame
        ret, img = cap.read()
        gray, rects = detect_face(img)
        # print("len of rects: ", len(rects))
        if len(rects) == 1:
            points1, _ = landmark_detection(gray, rects)
            filename2 = filename
            img2 = cv2.imread(filename2);
            points2 = readPoints(filename + '.txt')
            # print("Opening image file: ", filename2)
            # print("Opening feature file: ", filename + '.txt')
            output = swap_images(img2, img, points2, points1)
            cv2.imshow("Face Swapped", output)
        elif len(rects) == 2:
            points1, points2 = landmark_detection(gray, rects)
            output = swap_images(img, img, points2, points1)
            output = swap_images(img, output, points1, points2)
            cv2.imshow("Face Swapped", output)
        else:
            cv2.imshow("Face Swapped", img)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

