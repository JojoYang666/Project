% function imDst = boxfilter(imSrc, r)
function Intial_T=Initial_illumination(image)
 mmax_temp(:,:) = max(image(:,:,1),image(:,:,2));
mmax_temp(:,:) = max(image(:,:,3),mmax_temp(:,:));
% Intial_T(:,:,1)=mmax_temp;
% Intial_T(:,:,2)=mmax_temp;
% Intial_T(:,:,3)=mmax_temp;
Intial_T=mmax_temp;
end