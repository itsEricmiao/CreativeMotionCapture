import cv2
MAX = 100
title_window = 'hybrid image'
alpha = 10
beta = 10

def hybrid_image(filename1, filename2):
    # Read Image 1
    image_path_1 = filename1
    image_path_2 = filename2

    image1 = cv2.imread(image_path_1)
    image2 = cv2.imread(image_path_2)

    image1 = cv2.resize(image1, dsize = (300, 300))
    image2 = cv2.resize(image2, dsize = (300, 300))

    # High Frequency Image

    low_frequency_image1 = cv2.GaussianBlur(image1, (7, 7), beta)
    high_frequency_image1 = image1 - low_frequency_image1
    # Low Frequency Image
    low_frequency_image2 = cv2.GaussianBlur(image2, (7, 7), alpha)

    # img = high_frequency_image1 + low_frequency_image2
    hybrid_image =  high_frequency_image1 + low_frequency_image2
    hybrid_image = cv2.resize(hybrid_image, dsize = (600,600))
    cv2.imshow('Image1',image1)
    cv2.moveWindow('Image1', 0, 0)
    cv2.imshow('LowPass',low_frequency_image2)
    cv2.moveWindow('LowPass', 0, 300)
    cv2.imshow('HighPass', high_frequency_image1)
    cv2.moveWindow('HighPass', 300, 0)
    cv2.imshow('Image2', image2)
    cv2.moveWindow('Image2', 300, 300)
    cv2.imshow('Hybrid Image', hybrid_image)
    cv2.moveWindow('Hybrid Image', 600, 0)

    # Show some stuff
    cv2.waitKey(0)



