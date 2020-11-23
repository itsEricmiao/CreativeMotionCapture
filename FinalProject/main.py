from cv2 import CV_64FC3, CV_64F, CV_8UC3
from StaticHybrid import *
from Utilities import *

mypath = "funny"
hybridpath = "hybrid"

def interface():
    print("Welcome to Hybrid Image Program")
    print("Press 1: Face Swapping")
    print("Press 2: Static Hybrid Image")
    option = int(input("Enter your value: "))
    if option == 2:
        print("Below are a few images you can choose from: ")

        counter = 1;
        files = findFiles(hybridpath)
        for each in files:
            print(counter, " : ", each)
            counter = counter + 1
        val1 = int(input("Enter your image 1: "))
        val2 = int(input("Enter your image 2: "))
        if (val1 < 0 or val1 > counter or val2 < 0 or val2 > counter):
            print("INVALID INPUT")
            interface()
        else:
            filename1 = str(hybridpath) + "/" + str(files[val1-1])
            filename2 = str(hybridpath) + "/" + str(files[val2-1])
            hybrid_image(filename1, filename2)
        return "no"

    elif option == 1:
        print("Below are a few images you can choose from: ")
        counter = 1;
        files = findFiles(mypath)
        for each in files:
            print(counter, " : ", each)
            counter = counter + 1
        val = int(input("Enter your value: "))
        if ((val) < 0 or (val) > counter):
            print("INVALID INPUT")
            interface()
        else:
            filename = str(mypath) + "/" + str(files[val-1])
        return filename
    else:
        print("INVALID INPUT")
        interface()

if __name__ == '__main__' :
    filename = interface()
    if filename == "no":
        exit(0)
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

