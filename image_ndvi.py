# Usage: python image_ndvi.py <pic1> <pic2>
import cv2, numpy, os, sys

cv2.namedWindow('Pic A')
cv2.namedWindow('Pic B')
cv2.namedWindow('Pic ndvi')

image1 = cv2.imread(sys.argv[1], cv2.CV_LOAD_IMAGE_GRAYSCALE)
image2 = cv2.imread(sys.argv[2], cv2.CV_LOAD_IMAGE_GRAYSCALE)
img1_w, img1_h = image1.shape
img2_w, img2_h = image2.shape
if img1_w != img2_w or img1_h != img2_h:
	print "Invalid pictures"
	sys.exit()

array1 = bytearray(image1)
array2 = bytearray(image2)
# print image1.ndim
image_sum = numpy.array(array1) + numpy.array(array2)
image_sub = numpy.array(array1) - numpy.array(array2)
# = numpy.absolute(image_diff)
# image_diff = image_diff.reshape(img1_w, img1_h, img1_b)
image_ndvi = (image_sub / image_sum + 1) / 2 * 256
image_ndvi = image_ndvi.reshape(img1_w, img1_h)
cv2.imwrite('Pic_ndvi.png', image_ndvi)
image_ndvi= cv2.imread('Pic_ndvi.png')
cv2.imshow('Pic A', image1)
cv2.imshow('Pic B', image2)
cv2.imshow('Pic ndvi', image_ndvi)
cv2.waitKey(0)
