function gama_corr=gama1(I)
% [m,n]=size(I);
% % r=zeros(m,n,'uint8');
% r=zeros(m,n,'double');
% gama=0.8;
% p=255/255^(gama);p=(1/p)^(1/gama);
% for i=1:m
%     for j=1:n
% 
%         r(i,j)=p*I(i,j)^(1/gama);
%      
%     end
% end
% % gama_corr=double(r);
% gama_corr=r;
gama_corr=I.^0.5;

end