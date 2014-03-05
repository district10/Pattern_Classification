# Usage: python image_diff.py <pic1> <pic2>
import cv2, numpy, os, sys

cv2.namedWindow('Pic A')
cv2.namedWindow('Pic B')
cv2.namedWindow('Pic diff')

image1 = cv2.imread(sys.argv[1])
image2 = cv2.imread(sys.argv[2])
img1_w, img1_h, img1_b = image1.shape
img2_w, img2_h, img2_b = image2.shape
if img1_w != img2_w or img1_h != img2_h or img1_b != img2_b:
	print "Invalid pictures"
	sys.exit()

array1 = bytearray(image1)
array2 = bytearray(image2)
# print image1.ndim
image_diff = numpy.array(array1) - numpy.array(array2)
image_diff = numpy.absolute(image_diff)
image_diff = image_diff.reshape(img1_w, img1_h, img1_b)

cv2.imwrite('Pic_diff.png', image_diff)
image_diff= cv2.imread('Pic_diff.png')
cv2.imshow('Pic A', image1)
cv2.imshow('Pic B', image2)
cv2.imshow('Pic diff', image_diff)
cv2.waitKey(0)
