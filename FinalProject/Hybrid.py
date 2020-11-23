import cv2

MAX = 100
title_window = 'hybrid image'

def on_trackbar(val):
    alpha = val / MAX
    beta = ( 1.0 - alpha )
    hybrid_image = cv2.addWeighted(image1, alpha, image2, beta, 0.0)
    cv2.imshow(title_window, hybrid_image)

def extract_high_frequency(image, alpha, beta):
    high_frequency_image = image - extract_low_frequency(image, alpha)
    return high_frequency_image

def extract_low_frequency(image, beta):
    low_frequency_image = cv2.GaussianBlur(image, (7, 7), beta)
    return low_frequency_image

def hybrid_image_combine(high_frequency_image, low_frequency_image):
    hybridImage = high_frequency_image + low_frequency_image
    return hybridImage

def read_image(image_path):
    image = cv2.imread(image_path, 0)
    return image

def display_image(image, title):
    cv2.imshow(title, image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

### Image 1 ###

alpha = 10
beta = 10

# Read Image 1
image_path_2 = 'hybrid/cat.jpg'
image_path_1 = 'hybrid/dog.jpg'
image1 = read_image(image_path_1)
# cv2.imshow('Image1',image1)
# resized_image = cv2.resize(image, (249, 369))
# display_image(resized_image, 'Resized Image 1')


cv2.namedWindow(title_window)
trackbar_name = 'Alpha x %d' % MAX
cv2.createTrackbar(trackbar_name, title_window , 0, MAX, on_trackbar)




# High Frequency Image
high_frequency_image = extract_high_frequency(image1, alpha, beta)
cv2.imshow('HighPass',high_frequency_image)

# Read Image 2
image2 = read_image(image_path_2)
# cv2.imshow('Image2',image2)

# Low Frequency Image
low_frequency_image = extract_low_frequency(image2, alpha)
cv2.imshow('LowPass',low_frequency_image)


hybrid_image =  hybrid_image_combine(high_frequency_image, low_frequency_image)
cv2.imshow('Hybrid Image',hybrid_image)


# Show some stuff
on_trackbar(0)

cv2.waitKey(0)