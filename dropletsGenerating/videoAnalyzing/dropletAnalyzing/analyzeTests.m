function [analyzingRange, analyzedFrames, estimDropCount, flowType, allDropCount] =...
    analyzeTests(analyzingLimits, testCount, videoCount, dataCell,...
    allVideos, template, threshold, closeRadius, minPixels,...
    dropletsCountLimit, needleOnFrame)
if analyzingLimits(2) > testCount
    analyzingLimits(2) = testCount;
end
if analyzingLimits(1) > testCount
    analyzingLimits(1) = testCount;
end
% IMPORTANT FOR RETRIEVING TEST NUM: dataCell{analyzingRange(relTestNum)}
analyzingRange = analyzingLimits(1):analyzingLimits(2);
analyzingTestCount = length(analyzingRange);

analyzedFrames = cell(analyzingTestCount,1);
estimDropCount = cell(analyzingTestCount,1);
flowType = cell(analyzingTestCount,1);
allDropCount = cell(analyzingTestCount,1);
% For showing frames
showIndexes = zeros(2,max(videoCount),analyzingTestCount);
tiledlayout('flow','TileSpacing','tight');

for relTestNum = 1:analyzingTestCount
    disp(strcat(string(relTestNum),") TEST #",...
        dataCell{analyzingRange(relTestNum)}));
    analyzedFrames{relTestNum} =...
        cell(videoCount(analyzingRange(relTestNum)),1);
    estimDropCount{relTestNum} =...
        zeros(videoCount(analyzingRange(relTestNum)),1);
    flowType{relTestNum} =...
        strings(videoCount(analyzingRange(relTestNum)),1);
    allDropCount{relTestNum} =...
        cell(videoCount(analyzingRange(relTestNum)),1);

    for numRange = 1:videoCount(analyzingRange(relTestNum))
        disp(strcat("Crop #", string(numRange)));
        analyzedFrames{relTestNum}{numRange} =... 
            analyzeFrames(allVideos{analyzingRange(relTestNum)},...
            template{analyzingRange(relTestNum)}, ...
            numRange, threshold, closeRadius, minPixels);
        [estimDropCount{relTestNum}(numRange),...
            flowType{relTestNum}(numRange),...
            allDropCount{relTestNum}{numRange}] =...
                getEstimDropCount(analyzedFrames{relTestNum}{numRange},...
                dropletsCountLimit, needleOnFrame);
        % SHOW FRAMES
        maxDropCount = max(allDropCount{relTestNum}{numRange});
        if maxDropCount >= 1
            showIndexes(1,numRange,relTestNum) =...
                find(allDropCount{relTestNum}{numRange} >= 1,1,"first");
        elseif maxDropCount == 0
            showIndexes(1,numRange,relTestNum) =...
                find(allDropCount{relTestNum}{numRange} == 0,1,"first");
        end
        showIndexes(2,numRange,relTestNum) =...
            find(allDropCount{relTestNum}{numRange} ==...
            maxDropCount,1,"last");
        nexttile;
        montage({analyzedFrames{relTestNum}{numRange}{showIndexes(1,...
            numRange,relTestNum)}.binImg,...
            analyzedFrames{relTestNum}{numRange}{showIndexes(1,...
            numRange,relTestNum)}.img,...
            analyzedFrames{relTestNum}{numRange}{showIndexes(2,...
            numRange,relTestNum)}.binImg, ...
            analyzedFrames{relTestNum}{numRange}{showIndexes(2,...
            numRange,relTestNum)}.img}, 'Size', [2 2]);
        title({strcat("TEST #",dataCell{analyzingRange(relTestNum)},...
            ", crop #", string(numRange)),...
            strcat("start #",string(showIndexes(1,numRange,relTestNum)),...
            " / last maxDrop #", string(showIndexes(2,...
            numRange,relTestNum))), strcat("Drop count: ",...
            string(estimDropCount{relTestNum}(numRange)))});
            
    end
end
end