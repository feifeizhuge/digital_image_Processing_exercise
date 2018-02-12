M = [0 0 0 0 5 10;
     0 0 0 5 10 10;
     0 0 5 10 10 10;
     0 5 10 10 10 10];
 
% creat a 3*3 square used as structuring elements
SE=strel('square',3);
% use function image erosion
M2 = imerode(M,SE);
gradient = M - M2;

%%
M_expand = padarray(M,[1,1],'symmetric','both')
SE1 = ones(3,3)
M3 = imerode(M_expand,SE1);
M3_small = M3(2:5,2:7)
M2 - M3_small
%%
src = imread('original.jpg');
dst = imerode(src,SE);
dst = src - dst;
imshow(dst);
