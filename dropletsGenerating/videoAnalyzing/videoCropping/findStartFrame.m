function startFrame = findStartFrame(detrendEMD, pks,...
    frameLimit, detrThreshold, framesCount, diflocs, curCrop, upPeakLoc)
% find point with pks/2 height
if curCrop<= length(pks{1})
    tempFrame = find(detrendEMD(1:upPeakLoc,1)<pks{1}(curCrop)/2,1,"last");
else
    tempFrame = upPeakLoc;
end

if isempty(tempFrame)
    tempFrame = upPeakLoc;
end

startSearchFrame = tempFrame - frameLimit;
if startSearchFrame < 1
    startSearchFrame = 1;
end

% Find zero-EMD
zeroEMDFrame = startSearchFrame -1 +...
    find(detrendEMD(startSearchFrame:tempFrame,...
    1)>detrThreshold,1,"first");
 if isempty(zeroEMDFrame)
     zeroEMDFrame = framesCount;
 elseif zeroEMDFrame<1
     zeroEMDFrame = 1;
 end

 % Find difPeaks
 difPkFrame =...
     diflocs{1}(find((diflocs{1}>...
     startSearchFrame)&(diflocs{1}<...
     min(tempFrame,zeroEMDFrame)), 1,"last"));
 if isempty(difPkFrame)
     difPkFrame = framesCount;
 elseif difPkFrame<1
     difPkFrame = 1;
 end

 %disp(difPkFrame);

 startFrame = min(zeroEMDFrame,difPkFrame);

 if startFrame == framesCount
     startFrame = startSearchFrame;
 end
end