function counts = normImgHist(img)
% image pixel count
totalPixelCount = size(img,1)*size(img,2);
%normImhist create normalize hist
counts = imhist(img);
counts = counts ./ totalPixelCount;
end

