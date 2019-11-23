import cv2
  
image = cv2.imread('/home/vladko/CLionProjects/Mpi_hw1/pic.jpg')
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
newpic='home/vladko/Downloads/New.pgm'  
cv2.imshow('Original image',image)
cv2.imshow('Gray image', gray)
status = cv2.imwrite('/home/vladko/CLionProjects/Mpi_hw1/New.pgm' ,gray)  
cv2.waitKey(0)
cv2.destroyAllWindows()

print("Image written to file-system : ",status) 
