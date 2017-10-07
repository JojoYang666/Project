clc;
clear;
close all;
tic
input=imread('test_picture/Img3.bmp');

[imgh,imgw,imgp]=size(input) ;
img_size=imgh*imgw;
% % figure,imshow(uint8(input));
% % title('ԭͼ');

% ͼ��ת
 input=double(input);
  step=1
%  tic
 img =255-input;
%  toc
 % figure,imshow(uint8(img));
  %title('ԭͼ��ת');
% imwrite(uint8(img), 'Donginvert2.bmp');

%% -------------------- �������(Dong�㷨) --------------------------------------------
step=2
% tic
imgdark(:,:)=min(img(:,:,1),img(:,:,2)); 
imgdark(:,:)=min(imgdark(:,:),img(:,:,3));    %��������ɫͨ����ȡ��Сֵ
%--------------------------------
%  figure,imshow(uint8(imgdark));
%  title('��ͨ����Сֵ');

number=100;                                %Dong�㷨��ֻȡǰ100�����ص�
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
          imgdbw(i,j)=(imgdark(i,j)>=threshold);       % imgdbwָʾimgdark�лҶ�ֵ������ֵ�����ص�λ������Ϊ1�ĵ���Ǵ������
          
      end
  end
%------------------------------------
% opimgdbw=1-imgdbw;%��Ϊ������ĵ��λ��Ϊ0,����Ϊ������ĵ��λ��Ϊ1
% imgspot(:,:,1)=img(:,:,1).*opimgdbw+255*imgdbw(:,:,1);%�Ӻ�ǰ��Ĳ���Ϊ������ĵ�Ϊ1����Ϊ������ĵ�Ϊ0  �Ӻź����Ǵ������Ϊ255
% %����Ϊ�������Ϊ0  ����һ���--�������λ255  �Ǵ�������λ��Ϊ1
% imgspot(:,:,2)=img(:,:,2).*opimgdbw;%�������Ϊ0  ����Ϊ������ĵ��λ��Ϊ1
% imgspot(:,:,3)=img(:,:,3).*opimgdbw;%�������Ϊ0 ����Ϊ������ĵ��λ��Ϊ1
% figure,imshow(uint8(imgspot));                         %imgspot�ú�ɫ����������ȡֵ����
% % �����Ļ���������λ�� ֻ��rͨ��Ϊ255 ����Ϊ0 ����imspot�����ú�ɫ���������ȡֵ����
% title('��Χ');


airR=max(max(img(:,:,1).*imgdbw));
airG=max(max(img(:,:,2).*imgdbw));
airB=max(max(img(:,:,3).*imgdbw));
% airR=255;
% airG=255;
% airB=255;
air(:,:,1)=airR*ones(imgh,imgw);
air(:,:,2)=airG*ones(imgh,imgw);
air(:,:,3)=airB*ones(imgh,imgw);%����Ĵ�����
% toc
% imwrite(uint8(air), 'E:\�㷨���\ͼƬ\�ϺõĴ�����\DSC00399���ͼƬ\DSC00399_air.JPG');
%  figure,imshow(uint8(air));
%  title('air');


 %% Dong��͸���ʲ���
 
%   tic
 k=15;
 w0=0.8;
% ---------��һ�����ʱ  ��ԭɫͼ------------

imgar=ordfilt2(img(:,:,1)./airR,1,ones(k,k),'symmetric');
imgag=ordfilt2(img(:,:,2)./airG,1,ones(k,k),'symmetric');
imgab=ordfilt2(img(:,:,3)./airB,1,ones(k,k),'symmetric'); 

imgairdark(:,:)=min(imgar(:,:),imgag(:,:)); 
imgairdark(:,:)=min(imgairdark(:,:),imgab(:,:));   %��������ɫͨ����ȡ��Сֵ 

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

 
