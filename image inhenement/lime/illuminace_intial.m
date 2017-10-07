clc;close all;clear all;
tic
L=im2double(imread('test_picture/6.bmp'));
mmax_temp(:,:) = max(L(:,:,1),L(:,:,2));
mmax_temp(:,:) = max(L(:,:,3),mmax_temp(:,:));
T_init=mmax_temp;
T_ref=T_init.^0.6;
R=L./T_ref;
toc
% figure;imshow(T_ref);
figure;imshow(R);
imwrite(R,'intial.bmp');
imwrite(T_init,'intialT.bmp');
