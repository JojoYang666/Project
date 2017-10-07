% function temp= TempF(Wh,Wv,TGrah,TGrav)
function temp= TempF(Wh,Wv,TGrah,TGrav)
t= 0.0001;
lambda= 0.15;
H=Wh./max(abs(TGrah),t);
V=Wv./max(abs(TGrav),t);
H=-lambda*H;
V=-lambda*V;
[r,c]=size(H);
k=r*c;
H_temp=H(:);
V_temp=V(:);


%%
%%自己理解尝试
% [V_row,~]=size(H_temp);
% DT1=-1*ones(1,V_row);
% DT2=ones(1,V_row);
% B(:,1) = DT1;
% B(:,2) = DT2;
% WtempH=spdiags(H_temp, 0, k, k);
% Wtempv=spdiags(V_temp, 0, k, k);
% dh=[0,-1];
% Dh=spdiags(B,dh,k,k);
% dv=[0,1];
% Dv=spdiags(B,dv,k,k);
% tempv=Dv'*Wtempv*Dv;
% temph=Dh'*WtempH*Dh;
% temp=tempv+temph;
%%

%%--第一次尝试
% [H_row,~]=size(H_temp);
% [V_row,~]=size(V_temp);
% WtempH=sparse((1:H_row),(1:H_row),H_temp);
% Wtempv=sparse((1:V_row),(1:V_row),V_temp);
% DVT1=-1*ones(1,V_row);
% DVT2=ones(1,V_row-1);
% DV1=sparse([(1:V_row) (2:V_row)],[(1:V_row) (1:V_row-1)],[DVT1 DVT2]);
% DV2=sparse([(1:V_row) (1:V_row-1)],[(1:V_row) (2:V_row)],[DVT1 DVT2]);
% 
% 
% DHT1=-1*ones(1,H_row);
% DHT2=ones(1,H_row-1);
% DH1=sparse([(1:H_row) (1:H_row-1)],[(1:H_row) (2:H_row)],[DHT1 DHT2]);
% DH2=sparse([(1:H_row) (2:H_row)],[(1:H_row) (1:H_row-1)],[DHT1 DHT2]);
% 
% tempv=(DV1*Wtempv)*DV2;
% temph=DH2*(WtempH*DH1);
% % WtempH=diag(Wh./(abs(TGrah)+t));
% % Wtempv=diag(Wv./(abs(TGrav)+t));
% % tempv=conv2((conv2(Wtempv,[0;-1;1],'same')),[0,-1,1],'same');
% % temph=conv2((conv2(WtempH,[0,-1,1],'same')),[0;-1;1],'same');
% temp=tempv+temph;
% 
% 
%%



%%
%%根据论文尝试
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
    %%
end