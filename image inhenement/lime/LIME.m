clc;close all;clear all;
tic
L=im2double(imread('test_picture/6.bmp'));
mmax_temp(:,:) = max(L(:,:,1),L(:,:,2));
mmax_temp(:,:) = max(L(:,:,3),mmax_temp(:,:));
T_init=mmax_temp;
 Th=conv2(T_init,[1,-1],'same');
 Tv=conv2(T_init,[1,-1]','same');
 t=0.0001;
 %%
%  æÿ’Û∑®1
%  sigma=2;
% ksize = bitor(round(5*sigma),1);
% GKernal = fspecial('gaussian', [1,ksize], sigma);
% 
% wh=imfilter(Th,GKernal);
% whx=max(abs(wh),t);
% wh=imfilter(1./whx,GKernal);
% wv=imfilter(Tv,GKernal);
% wvx=max(abs(wv),t);
% wv=imfilter(1./wvx,GKernal);
%%

%%
% %æÿ’Û∑®2
% wv=1;
% wh=1;
%%


%%
%æÿ’Û∑®3
wh=1./max(abs(Th),t);
wv=1./max(abs(Tv),t);
%%







%%

lambda= 0.15;
H=wh./max(abs(Th),t);
V=wv./max(abs(Tv),t);
H=-lambda*H;
V=-lambda*V;
[r,c]=size(H);
k=r*c;
H_temp=H(:);
V_temp=V(:);
dx=H_temp;
dy=V_temp;
 B(:,1) = dx;
 B(:,2) = dy;


 d = [-r,-1];
    A = spdiags(B,d,k,k);
    e = dx;
    w = padarray(dx, r, 'pre'); w = w(1:end-r);
    s = dy;
    n = padarray(dy, 1, 'pre'); n = n(1:end-1);
    D = 1-(e+w+s+n);
    A = A + A' + spdiags(D, 0, k, k); 
    temp=A;
    
    T_refin=temp\T_init(:);
T_refin=reshape(T_refin,size(T_init));

T_ref=T_refin.^0.6;
R=L./T_ref;
toc
% figure;imshow(T_ref);
figure;imshow(R);
imwrite(R,'LImeSP.bmp');
imwrite(T_refin,'LImeSPT.bmp');