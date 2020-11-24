from Utilities import *
mypath = "images"

# https://stackoverflow.com/questions/51166970/how-can-i-check-if-only-txt-file-exists-in-a-directory-with-python

if __name__ == '__main__' :
    onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]

    for each in onlyfiles:
        if each.endswith(".jpg"):
            print(each)
            img = cv2.imread(mypath + "/" + each)
            gray, rects = detect_face(img)
            points1, _ = landmark_detection(gray, rects)
            with open(mypath + "/" + each+'.txt', 'w') as fp:
                fp.write('\n'.join('%s %s' % x for x in points1))