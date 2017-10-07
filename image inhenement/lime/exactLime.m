clc;close all;clear all;
tic
L=im2double(imread('test_picture/f.JPG'));
lambda = .15;
sigma = 2; 
strategy = 3;
ts=Initial_illumination(L);
T_ref = exactS(ts,lambda,sigma,strategy);
T_ref=gama1(T_ref);
R=L./T_ref;
toc
% figure;imshow(T_ref);
figure;imshow(R);