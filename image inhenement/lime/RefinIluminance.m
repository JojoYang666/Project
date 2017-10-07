function T_refin=RefinIluminance(temp,T_init)
% [row,cow]=size(temp);
% I=sparse((1:row),(1:cow),ones(1,row));
% T_refin=(I+temp)\T_init(:);
% T_refin=reshape(T_refin,size(T_init));


% 
T_refin=temp\T_init(:);
T_refin=reshape(T_refin,size(T_init));
end