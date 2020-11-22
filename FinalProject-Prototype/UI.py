import cv2

def face_detection():
    # Load the cascade
    face_cascade = cv2.CascadeClassifier('haarcascades/haarcascade_frontalface_default.xml')
    # To capture video from webcam.
    cap = cv2.VideoCapture(0)
    # To use a video file as input
    # cap = cv2.VideoCapture('filename.mp4')

    while True:
        # Read the frame
        _, img = cap.read()
        # img = cv2.imread('image.jpg')
        # Convert into grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        # Detect the faces
        faces = face_cascade.detectMultiScale(gray, 1.1, 4)
        # Draw the rectangle around each face
        for (x, y, w, h) in faces:
            cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)

        # Display
        cv2.imshow('img', img)
        # Stop if escape key is pressed
        k = cv2.waitKey(30) & 0xff
        if k == 27:
            break
    # Release the VideoCapture object
    cap.release()

def feature_detection():
    cascPath = "haarcascades/haarcascade_frontalface_default.xml"
    eyePath = "haarcascades/haarcascade_eye.xml"
    smilePath = "haarcascades/haarcascade_smile.xml"

    faceCascade = cv2.CascadeClassifier(cascPath)
    eyeCascade = cv2.CascadeClassifier(eyePath)
    smileCascade = cv2.CascadeClassifier(smilePath)

    font = cv2.FONT_HERSHEY_SIMPLEX
    video_capture = cv2.VideoCapture(0)

    while True:
        # Capture frame-by-frame
        ret, frame = video_capture.read()

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        faces = faceCascade.detectMultiScale(
            gray,
            scaleFactor=1.1,
            minNeighbors=5,
            minSize=(200, 200),
            flags=cv2.CASCADE_SCALE_IMAGE
        )

        # Draw a rectangle around the faces
        for (x, y, w, h) in faces:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 3)
            roi_gray = gray[y:y + h, x:x + w]
            roi_color = frame[y:y + h, x:x + w]
            cv2.putText(frame, 'Face', (x, y), font, 2, (255, 0, 0), 5)

        smile = smileCascade.detectMultiScale(
            roi_gray,
            scaleFactor=1.16,
            minNeighbors=35,
            minSize=(25, 25),
            flags=cv2.CASCADE_SCALE_IMAGE
        )

        for (sx, sy, sw, sh) in smile:
            cv2.rectangle(roi_color, (sh, sy), (sx + sw, sy + sh), (255, 0, 0), 2)
            cv2.putText(frame, 'Smile', (x + sx, y + sy), 1, 1, (0, 255, 0), 1)

        eyes = eyeCascade.detectMultiScale(roi_gray)
        for (ex, ey, ew, eh) in eyes:
            cv2.rectangle(roi_color, (ex, ey), (ex + ew, ey + eh), (0, 255, 0), 2)
            cv2.putText(frame, 'Eye', (x + ex, y + ey), 1, 1, (0, 255, 0), 1)

        cv2.putText(frame, 'Number of Faces Detected: ' + str(len(faces)), (40, 40), font, 1, (255, 0, 0), 2)
        # Display the resulting frame
        cv2.imshow('Video', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


video_capture = cv2.VideoCapture(0)
font = cv2.FONT_HERSHEY_SIMPLEX
row = 720
col = 1280
background = cv2.imread("background.jpeg")
x = cv2.imread("image.jpg")
y = cv2.imread("image1.jpg")
z = cv2.imread("image2.jpg")

background = cv2.resize(background, (col,row))
x = cv2.resize(x,(col,row))
y = cv2.resize(y,(col,row))
z = cv2.resize(z,(col,row))

while True:
    # Capture frame-by-frame

    # ret, background = video_capture.read()

    cv2.putText(background, 'Welcome to Hybrid Image Program', (40, 40), font, 1, (255, 255, 255), 2)
    cv2.putText(background, 'Press 1: Face Detection', (40, 80), font, 1, (255, 255, 255), 2)
    cv2.putText(background, 'Press 2: Feature Detection', (40, 120), font, 1, (255, 255, 255), 2)
    cv2.putText(background, 'Press 3: Show Hybrid Images', (40, 160), font, 1, (255, 255, 255), 2)

    cv2.imshow('Interface', background)
    cv2.moveWindow('Interface', 0, 0)
    if cv2.waitKey(0) & 0xFF == ord('1'):
        cv2.imshow('1', y)
        cv2.moveWindow('1', 0, 0)
    if cv2.waitKey(0) & 0xFF == ord('2'):
        face_detection()
    if cv2.waitKey(0) & 0xFF == ord('3'):
        feature_detection()



