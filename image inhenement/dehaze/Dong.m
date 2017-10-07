clc;
clear;
close all;
tic
input=imread('test_picture/Img3.bmp');

[imgh,imgw,imgp]=size(input) ;
img_size=imgh*imgw;
% % figure,imshow(uint8(input));
% % title('原图');

% 图像反转
 input=double(input);
  step=1
%  tic
 img =255-input;
%  toc
 % figure,imshow(uint8(img));
  %title('原图反转');
% imwrite(uint8(img), 'Donginvert2.bmp');

%% -------------------- 求大气光(Dong算法) --------------------------------------------
step=2
% tic
imgdark(:,:)=min(img(:,:,1),img(:,:,2)); 
imgdark(:,:)=min(imgdark(:,:),img(:,:,3));    %在三个颜色通道中取最小值
%--------------------------------
%  figure,imshow(uint8(imgdark));
%  title('三通道最小值');

number=100;                                %Dong算法中只取前100个像素点
h=imhist(uint8(imgdark));
 s=0;
  for i=256:-1:1
      if (s<=number)
        s=s+h(i);
        threshold=i-1;
      else break;
      end
  end
imgdbw= zeros(imgh,imgw);  
  for i=1:imgh
      for j=1:imgw
          imgdbw(i,j)=(imgdark(i,j)>=threshold);       % imgdbw指示imgdark中灰度值大于阈值的像素点位，凡是为1的点就是大气光点
          
      end
  end
%------------------------------------
% opimgdbw=1-imgdbw;%作为大气光的点的位置为0,不作为大气光的点的位置为1
% imgspot(:,:,1)=img(:,:,1).*opimgdbw+255*imgdbw(:,:,1);%加号前面的不作为大气光的点为1，作为大气光的点为0  加号后面是大气光点为255
% %不作为大气光点为0  这样一相加--大气光点位255  非大气光点的位置为1
% imgspot(:,:,2)=img(:,:,2).*opimgdbw;%大气光点为0  不作为大气光的点的位置为1
% imgspot(:,:,3)=img(:,:,3).*opimgdbw;%大气光点为0 不作为大气光的点的位置为1
% figure,imshow(uint8(imgspot));                         %imgspot用红色标出大气光的取值区域
% % 这样的话大气光点的位置 只有r通道为255 其余为0 所以imspot就能用红色标出大气光取值区域
% title('范围');


airR=max(max(img(:,:,1).*imgdbw));
airG=max(max(img(:,:,2).*imgdbw));
airB=max(max(img(:,:,3).*imgdbw));
% airR=255;
% airG=255;
% airB=255;
air(:,:,1)=airR*ones(imgh,imgw);
air(:,:,2)=airG*ones(imgh,imgw);
air(:,:,3)=airB*ones(imgh,imgw);%求出的大气光
% toc
% imwrite(uint8(air), 'E:\算法相关\图片\较好的处理结果\DSC00399相关图片\DSC00399_air.JPG');
%  figure,imshow(uint8(air));
%  title('air');


 %% Dong求透射率操作
 
%   tic
 k=15;
 w0=0.8;
% ---------这一段最耗时  求暗原色图------------

imgar=ordfilt2(img(:,:,1)./airR,1,ones(k,k),'symmetric');
imgag=ordfilt2(img(:,:,2)./airG,1,ones(k,k),'symmetric');
imgab=ordfilt2(img(:,:,3)./airB,1,ones(k,k),'symmetric'); 

imgairdark(:,:)=min(imgar(:,:),imgag(:,:)); 
imgairdark(:,:)=min(imgairdark(:,:),imgab(:,:));   %在三个颜色通道中取最小值 

t=  1-w0*imgairdark; 
% figure;imshow(t);title('t0');
% imwrite(t, 'darkt.bmp');


 for i=1:imgh
      for j=1:imgw
          if t(i,j) > 0&& t(i,j)<0.5
              t(i,j) = 2 *t(i,j)*t(i,j);
          end
          
      end
 end
  
 

%  toc
%   figure;imshow(t);title('t');
%   imwrite(t, 't2.bmp');
step=4
%  tic
obj(:,:,1)=(img(:,:,1)-airR)./t+airR;
obj(:,:,2)=(img(:,:,2)-airG)./t+airG;
obj(:,:,3)=(img(:,:,3)-airB)./t+airB;
obj=max(min(obj,255),0);
%  toc

% imwrite(uint8(obj),'Dongj2.bmp');

objinverted = 255-obj;
toc
figure,imshow(uint8(objinverted));
title('objinverted');
% imwrite(uint8(objinverted),'Dongresult5.bmp');

 
