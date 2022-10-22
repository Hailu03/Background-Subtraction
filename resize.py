import cv2

img = cv2.imread('images/background.jpg')
# img = cv2.resize(img,(964,699))
cv2.imwrite("images/background_resize.png",img)
print(img.shape)