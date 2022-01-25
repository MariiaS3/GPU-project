import cv2;

data = cv2.imread("hm.tif", flags=(cv2.IMREAD_GRAYSCALE | cv2.IMREAD_ANYDEPTH))

output_file = open('hm.raw', 'wb')
data.tofile(output_file)
output_file.close()
