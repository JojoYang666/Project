clc;close all;clear all;
tic
L=im2double(imread('test_picture/Img3.bmp'));
T_init=Initial_illumination(L);
%  [Th,Tv]=gradient(T_init);给T差分 --法1


%%
%%给T差分 --法二
%  Th = diff(T_init,1,2);
%  Th = padarray(Th, [0 1 0], 'post');
%  Tv = diff(T_init,1,1);
%  Tv = padarray(Tv, [1 0 0], 'post');
 %%
 
 %%
 %%给T差分 --法三
 Th=conv2(T_init,[1,-1],'same');
 Tv=conv2(T_init,[1,-1]','same');
 
 
 %%
 
 [Wh,Wv] = Weight(Th,Tv);
 temp= TempF(Wh,Wv,Th,Tv);
 T_refin=RefinIluminance(temp,T_init);
%  figure;imshow(T_refin);
T_ref=gama1(T_refin);
% 
% R(:,:,1)=L(:,:,1)./T_ref;
% R(:,:,2)=L(:,:,2)./T_ref;
% R(:,:,3)=L(:,:,3)./T_ref;
R=L./T_ref;
toc
% figure;imshow(T_ref);
figure;imshow(R);