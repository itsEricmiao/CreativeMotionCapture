# Some Reference:
# http://dlib.net/face_landmark_detection.py.html
# https://towardsdatascience.com/facial-mapping-landmarks-with-dlib-python-160abcf7d672
# https://rajathithanrajasekar.medium.com/opencv-series-4-dlib-68-point-face-landmark-prediction-172cb867c869

from cv2 import CV_64FC3, CV_64F, CV_8UC3
from StaticHybrid import *
from Utilities import *
from FeatureTracking import *

# relative file path to my images
funnypath = "funny"
hybridpath = "hybrid"
imagepath = "images"

# user interface (it is bad I know...)
def interface():
    print("Welcome to Hybrid Image Program")
    print("Press 1: Face and Feature Trackings")
    print("Press 2: Face Swapping")
    print("Press 3: Static Hybrid Image")
    option = int(input("Enter your value: "))
    if option == 1:
        feature_tracking()
        return "no"
    elif option == 2:
        print("Please select image category:")
        print("1: Celebrities")
        print("2: Funny")
        opt = int(input("Enter your value: "))
        if opt == 1:
            path = imagepath
        elif opt == 2:
            path = funnypath
        print("Below are a few images you can choose from: ")

        counter = 1;
        files = findFiles(path)
        for each in files:
            print(counter, " : ", each)
            counter = counter + 1
        val = int(input("Enter your value: "))
        if ((val) < 0 or (val) > counter):
            print("INVALID INPUT")
            interface()
        else:
            filename = str(path) + "/" + str(files[val - 1])
        return filename

    elif option == 3:
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
            filename1 = str(hybridpath) + "/" + str(files[val1 - 1])
            filename2 = str(hybridpath) + "/" + str(files[val2 - 1])
            hybrid_image(filename1, filename2)
        return "no"

    else:
        print("INVALID INPUT")
        interface()

if __name__ == '__main__' :
    background = cv2.imread("background.jpeg")
    font = cv2.FONT_HERSHEY_SIMPLEX
    row = 720
    col = 1280
    background = cv2.resize(background, (col, row))
    cv2.putText(background, 'Welcome to Hybrid Image Program', (40, 40), font, 1, (255, 255, 255), 2)
    cv2.putText(background, 'Press 1: Face and Features Tracking', (40, 80), font, 1, (255, 255, 255), 2)
    cv2.putText(background, 'Press 2: Face Swapping', (40, 120), font, 1, (255, 255, 255), 2)
    cv2.putText(background, 'Press 3: Show Hybrid Images', (40, 160), font, 1, (255, 255, 255), 2)
    cv2.imshow("App", background)
    cv2.waitKey(0)
    filename = interface()
    if filename == "no":
        exit(0)
    # Read images
    cap = cv2.VideoCapture(0)
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
            output = swap_images(img2, img, points2, points1)
            cv2.imshow("Face Swapped", output)
        else:
            cv2.imshow("Face Swapped", img)
        # press q to exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

