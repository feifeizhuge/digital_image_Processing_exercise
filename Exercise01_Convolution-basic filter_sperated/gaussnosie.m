img = imread('original.jpg'); 
img_noise = imnoise(img,'gaussian');
imwrite(img_noise,'img_gaussian.jpg');