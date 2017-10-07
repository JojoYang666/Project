clc;
clear;
close all; 
%计时函数
tic

%读入图像
img=imread('test_picture/9.bmp');
img=255-img;
img=double(img);%一般情况下 读入的图片是unit8的格式,进行运算的时候，为了防止溢出，一般是转成double的类型，等处理完的矩阵
                %如果要imshow,再转成unit8

[imgh,imgw,imgc]=size(img) ;
% img_size=imgh*imgw;%图像的大小

 k_air=15;
 k_t=15; %处理窗口的大小
%  w0=0.95;%增加真实感的系数
 t0=0.1;%transmission的下限
 
Y(:,:) = 0.299*img(:,:,1)+0.587*img(:,:,2)+0.114*img(:,:,3) ;%%亮度分量，与rgb2gray函数使用相同的权重。\

  air=max(max(Y));

CY = 1.08-Y/255;       %%常量越小，结果越亮


rt = medfilt2(CY,[15,15]);%中值滤波
rt = max(rt,0.01);



 obj(:,:,1)=(img(:,:,1)-air)./rt+air;
obj(:,:,2)=(img(:,:,2)-air)./rt+air;
obj(:,:,3)=(img(:,:,3)-air)./rt+air;
  
obj=max(min(obj,255),0);
 obj=255-obj;
  toc
 figure,imshow(uint8(obj));
 imwrite(uint8(obj),'test33.bmp');

