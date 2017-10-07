clc;close all;clear all;
tic
img=im2double(imread('test_picture/6.bmp'));
k_air=10;
imgr=ordfilt2(img(:,:,1),k_air*k_air,ones(k_air,k_air),'symmetric');
imgg=ordfilt2(img(:,:,2),k_air*k_air,ones(k_air,k_air),'symmetric');
imgb=ordfilt2(img(:,:,3),k_air*k_air,ones(k_air,k_air),'symmetric');          %�ֱ��������ɫͨ����15��15�ĺ�ȡ��Сֵ
imgdark(:,:)=min(imgr(:,:),imgg(:,:)); 
imgdark(:,:)=min(imgdark(:,:),imgb(:,:)); 
imgdark=bilateralFilter(imgdark);
T_ref=imgdark.^0.6;
R=img./T_ref;
figure;imshow(R);
imwrite(R,'ilBF.bmp');
imwrite(imgdark,'ilBFT.bmp');
toc
