function [wh,wv] = Weight(Th,Tv)
t=0.0001;
% wh=nlfilter(Th,[10,10],@funh);
% wv=nlfilter(Tv,[10,10],@funv);
sigma=2;
ksize = bitor(round(5*sigma),1);
GKernal = fspecial('gaussian', [1,ksize], sigma);

wh=imfilter(Th,GKernal);
whx=max(abs(wh),t);
wh=imfilter(1./whx,GKernal);
wv=imfilter(Tv,GKernal);
wvx=max(abs(wv),t);
wv=imfilter(1./wvx,GKernal);



% wh=conv2(Th,GKernal,'same');
% whx=max(abs(wh),t);
% wh=conv2(1./whx,GKernal,'same');
% wv=conv2(Tv,GKernal,'same');
% wvx=max(abs(wv),t);
% wv=conv2((1./wvx),GKernal,'same');
end


