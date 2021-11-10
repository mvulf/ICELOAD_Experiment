function endFrame = findEndFrame(detrendEMD, pks, frameLimit,...
    framesCount, detrThreshold, diflocs, curCrop, downPeakLoc)
if curCrop<= length(pks{1})
    % find point with pks/2 height
    tempFrame =  downPeakLoc-1 + find(detrendEMD(downPeakLoc:end,...
        2)<pks{2}(curCrop)/2,1,"first");
else
    tempFrame = downPeakLoc;
end

if isempty(tempFrame)
    tempFrame = downPeakLoc;
end

endSearchFrame = tempFrame+frameLimit;
if endSearchFrame > framesCount
    endSearchFrame = framesCount;
end

% find zero-EMD
zeroEMDFrame = tempFrame-1 +...
    find(detrendEMD(tempFrame:endSearchFrame,...
    2)>detrThreshold,1,"last");
if isempty(zeroEMDFrame)
    zeroEMDFrame = 0;
elseif zeroEMDFrame>framesCount
    zeroEMDFrame = framesCount;
end
% find difPeaks
difPkFrame =...
    diflocs{2}(find((diflocs{2}<...
    endSearchFrame)&(diflocs{2}>...
    max(tempFrame,zeroEMDFrame)), 1,"first"));
if isempty(difPkFrame)
    difPkFrame = 0;
elseif difPkFrame>framesCount
    difPkFrame = framesCount;
end

endFrame = max(zeroEMDFrame,difPkFrame);

if endFrame == 0
    endFrame = endSearchFrame;
end
end