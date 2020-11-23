from cv2 import CV_64FC3, CV_64F, CV_8UC3

from Utilities import *

mypath = "funny"
hybridpath = "hybrid"
MAX = 100;
sigma = 5;
image1 = ();
image2 = ();
hybrid_image = ();
low_freq_img1 = ();
high_freq_img1 = ();
low_freq_img2 = ();
high_freq_img2 = ();

def hybrid_image():
    print("Welcome to Static Hybrid Image Section")
    print("Below are a few images you can choose from: ")
    files = findFiles(hybridpath)
    counter = 1;
    for each in files:
        print(counter, " : ", each)
        counter = counter + 1
    img1 = int(input("Enter your first image: "))
    img2 = int(input("Enter your second image: "))
    if (img1 < 0 or img1 > counter-1 or img2 < 0 or img2 > counter-1):
        print("INVALID INPUT")
        interface()
    else:
        filename1 = str(hybridpath) + "/" + str(files[img1 - 1])
        filename2 = str(hybridpath) + "/" + str(files[img2 - 1])
        x = cv2.imread(filename1)
        y = cv2.imread(filename2)

        image1 = cv2.cvtColor(x, CV_64FC3)
        image2 = cv2.cvtColor(y, CV_64FC3)

        image1 = cv2.resize(image1, dsize = (300,300), interpolation =cv2.INTER_AREA);
        image2 = cv2.resize(image2, dsize = (300,300), interpolation =cv2.INTER_AREA);

        n = sigma * 2 + 1;
        filter = cv2.getGaussianKernel(n, sigma, CV_64F);

        low_freq_img1 = cv2.filter2D(image1, -1, filter);
        low_freq_img2 = cv2.filter2D(image2, -1, filter);

        high_freq_img2 = image2 - low_freq_img2;
        hybrid_image = low_freq_img1 + high_freq_img2;

        # high_freq_img2 = cv2.cvtColor(high_freq_img2, CV_8UC3);
        # low_freq_img1 = cv2.cvtColor(low_freq_img1, CV_8UC3);
        print("type: ", hybrid_image.type())
        hybrid_image = cv2.cvtColor(hybrid_image, CV_8UC3);
        print("type: ", hybrid_image.type())

        # cv2.imshow("Low frequencies", low_freq_img1);
        # cv2.moveWindow("Low frequencies", 300, 0);
        # cv2.imshow("High frequencies", high_freq_img2);
        # cv2.moveWindow("High frequencies", 600, 0);
        cv2.imshow("Hybrid Image", hybrid_image);
        # cv2.waitKey(1)

def interface():
    print("Welcome to Hybrid Image Program")
    print("Press 1: Face Swapping")
    print("Press 2: Static Hybrid Image")
    option = int(input("Enter your value: "))
    if option == 2:
        hybrid_image()
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

