clc;
clear;
close all; 
%��ʱ����
tic

%����ͼ��
img=imread('test_picture/9.bmp');
img=255-img;
img=double(img);%һ������� �����ͼƬ��unit8�ĸ�ʽ,���������ʱ��Ϊ�˷�ֹ�����һ����ת��double�����ͣ��ȴ�����ľ���
                %���Ҫimshow,��ת��unit8

[imgh,imgw,imgc]=size(img) ;
% img_size=imgh*imgw;%ͼ��Ĵ�С

 k_air=15;
 k_t=15; %�����ڵĴ�С
%  w0=0.95;%������ʵ�е�ϵ��
 t0=0.1;%transmission������
 
Y(:,:) = 0.299*img(:,:,1)+0.587*img(:,:,2)+0.114*img(:,:,3) ;%%���ȷ�������rgb2gray����ʹ����ͬ��Ȩ�ء�\

  air=max(max(Y));

CY = 1.08-Y/255;       %%����ԽС�����Խ��


rt = medfilt2(CY,[15,15]);%��ֵ�˲�
rt = max(rt,0.01);



 obj(:,:,1)=(img(:,:,1)-air)./rt+air;
obj(:,:,2)=(img(:,:,2)-air)./rt+air;
obj(:,:,3)=(img(:,:,3)-air)./rt+air;
  
obj=max(min(obj,255),0);
 obj=255-obj;
  toc
 figure,imshow(uint8(obj));
 imwrite(uint8(obj),'test33.bmp');

