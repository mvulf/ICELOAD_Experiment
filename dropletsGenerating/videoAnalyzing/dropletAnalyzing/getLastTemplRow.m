function [lastTemplRow, binTempl] =...
    getLastTemplRow(template,noizeKoef,strTest)
%GETLASTTEMPLROW find last row with needle or static droplet pixels
% get inverse image
[~, invTempl] = imlocalbrighten(template, 1);

% template threshold by taking maximum from columns outliers
Q1 = quantile(invTempl(:,:),0.25,2);
Q3 = quantile(invTempl(:,:),0.75,2);
IQR = Q3-Q1;

templThresh = max(Q3 + noizeKoef*IQR);
if templThresh == 0
    templThresh = noizeKoef/10;
    if templThresh > 0.9
        templThresh = 0.9;
    end
end

% get binary template image
binTempl = imbinarize(invTempl,templThresh);

% Morphological structuring element - disk
% disk mask decomposition
decomposition = 8;
closeRadius = 20;
se = strel('disk', closeRadius, decomposition);

% close mask with disk
binTempl = imclose(binTempl, se);    
% delete objects with low count of pixels
minPixels = 50;
binTempl = bwareaopen(binTempl, minPixels);
% fill holes
binTempl = imfill(binTempl, 'holes');

[lastTemplRow,~] = find(max(binTempl,[],2),1,'last');
nexttile;
imshow(binTempl(1:lastTemplRow,:));
title(strcat("Test #", strTest));

end

