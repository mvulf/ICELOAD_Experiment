function [estimDropCount, flowType, allDropCount] =...
    getEstimDropCount(analyzedFrames, dropletsCountLimit, needleOnFrame)
% getting estimated count of droplets
allDropCount = zeros(length(analyzedFrames),1);

if needleOnFrame
    substrNeedle = 1;
else
    substrNeedle = 0;
end

parfor i=1:length(analyzedFrames) %parfor
    allDropCount(i) = analyzedFrames{i}.dropCount-substrNeedle;
end

estimDropCount = max(allDropCount);

%estimDropCount = quantile(allDropCount,0.75)
% if max(allDropCount) <= 1
%     estimDropCount = max(allDropCount);
% else
%     estimDropCount = max(max(allDropCount)-1, median(allDropCount));
% end

% Getting flow type
if estimDropCount == 0
    flowType = "No";
elseif estimDropCount == 1
    flowType = "Single";
elseif estimDropCount <= dropletsCountLimit
    flowType = "Multi";
else
    flowType = "Jet";
end

disp(strcat("Estimated droplets count: ", sprintf("%d",estimDropCount)));
disp(strcat("Flow type: ", flowType));
end