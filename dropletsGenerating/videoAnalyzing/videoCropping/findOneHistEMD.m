function totalEMD = findOneHistEMD(h1, threshold, upScaleKoef,...
downScaleKoef)
% This function calculates EMD (Earth moving distance) 
% between intensity histogram and black picture
% let us use piecewise calculation. Increasing influence in light tones.
% Freshould, after witch influence is increasing
% ----------------------------
% overloading defaults values
sizeH1 = size(h1);

% get intencity range
intencityRange = 1:sizeH1;

% hist differ with black image
h1(1) = h1(1) - 1.0;

% product
EMD = h1.*intencityRange';

% calcuclating distance
totalEMD = downScaleKoef*sum(EMD(1:threshold)) +...
    upScaleKoef*sum(EMD(threshold+1:sizeH1));
end
