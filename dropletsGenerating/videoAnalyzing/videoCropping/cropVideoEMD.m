function [filtSelectedFrames, analyzedFrames, returnEMD,...
    templateImg] = cropVideoEMD(obj, video, frameRange, threshold,...
    upScaleKoef, downScaleKoef, frameLimit, smoothSpan, polyOrder,...
    baseFrameSide, maxEMDQuantileValue, maxSecDifQuantileValue,...
    UpDownKoef, quantileThreshold)
%CROPVIDEO import video and analyze to finding onehistEMD
%   frameRange necessary if not whole video is analyzing
%   return EMD-values and selected frames
%   also return videofile and it's characteristics
%----------------------------------------------------

% chosing frame count
framesCount = frameRange(2) + 1 - frameRange(1);

% full sizes
height = obj.Height;
width = obj.Width;

% NEW GETTING PARTS 
% [height width] of part-picture
% cell - up, down
partSizes = {[floor(height/2) width], [floor(height/2) width]};

% imgCropRange{i} = [xmin ymin width height] for imcrop(Im,imgCropRange{i})
% cell - up, down
imgCropRange = {[1 1 flip(partSizes{1})],...
    [1 partSizes{1}(1)+1 flip(partSizes{2})]};

% creating EMD array - up, down
EMD = zeros(framesCount,2);

% Get template
medianFrameLimit = 2000;
% medianTemplateImg is
if framesCount < medianFrameLimit
    templateImg = uint8(im2gray(median((video(:,:,:,:)),4)));
else
    medianRangeCount = ceil(framesCount/medianFrameLimit);
    subTemplateImg = uint8(zeros(height,width,medianRangeCount));
    % getting mediansTemplates
    for medianNum = 1:1:medianRangeCount
        lastFrame = medianNum*medianFrameLimit;
        if lastFrame>framesCount
            lastFrame = framesCount;
        end
        subTemplateImg(:,:,medianNum) = uint8(im2gray(median((video(:,...
            :,:,(medianNum-1)*medianFrameLimit+1:...
            lastFrame)),4)));
    end
    % get template
    templateImg = uint8(median(subTemplateImg(:,:,:),3));
end
    
% Calculate EMD for all frames
for frameNum=1:framesCount
    % get frame
    frame = im2gray(video(:,:,:,frameNum));
    % get frame differ with template
    frameDiff = imabsdiff(frame,templateImg);
    % Parallel calculating of EMD values
    parfor side = 1:2
        EMD(frameNum,side) = findOneHistEMD(normImgHist(imcrop(frameDiff,...
           imgCropRange{side})),threshold, upScaleKoef, downScaleKoef);
    end
end

% return analyzed frames
analyzedFrames = frameRange(1):frameRange(2);

% NEW ANALYZING
pks = cell(2,1);
locs = cell(2,1);
difPks = cell(2,1);
diflocs = cell(2,1);

smoothedEMD = zeros(framesCount,2);
detrendEMD = zeros(framesCount,2);
secDerEMD = zeros(framesCount-2,2);

% DETRENDING and getting SECOND DERIVATIVE
parfor side = 1:2
    smoothedEMD(:,side) = smooth(EMD(:,side),smoothSpan);
    % EMD detrending
    if polyOrder(side) >= 0
        [pEMD,sEMD,muEMD] = polyfit((1:numel(smoothedEMD(:,side)))',...
            smoothedEMD(:,side),polyOrder(side));
        f_EMD = polyval(pEMD,(1:numel(smoothedEMD(:,side)))',[],muEMD);
        
        % detrend data
        detrendEMD(:,side) = smoothedEMD(:,side) - f_EMD;
    else
        detrendEMD(:,side) = smoothedEMD(:,side);
    end
    % second derivative
    secDerEMD(:,side) = diff(diff(detrendEMD(:,side)));

    % main peaks
    [pks{side}, locs{side}] = findpeaks(detrendEMD(:,side),...
    analyzedFrames,'MinPeakDistance',frameLimit,...
    'MinPeakHeight',quantile(detrendEMD(:,side),...
    maxEMDQuantileValue(side)));

    % diff peaks
    [difPks{side}, diflocs{side}] = findpeaks(secDerEMD(:,side),...
    analyzedFrames(3:end), 'MinPeakHeight', ...
    quantile(secDerEMD(:,side), maxSecDifQuantileValue(side)));

end

% If droplets falling doesn't register - change side
if (numel(pks{baseFrameSide}) == 0)||...
        (numel(pks{baseFrameSide})>numel(pks{3 - baseFrameSide}))
    baseFrameSide = 3 - baseFrameSide;
end

cropCount = numel(pks{baseFrameSide});
disp(strcat("Count of detected crops: ", string(cropCount)));

% SELECTING FRAMES
selectedFrames = zeros(cropCount,2);

% get thresholds
detrThreshold = zeros(2,1);
if quantileThreshold(1)<=1
    detrThreshold(1) = quantile(rmoutliers(detrendEMD(:,1)),quantileThreshold(1));
else%if quantileThreshold(1)<=2
    %detrThreshold(1) = (quantileThreshold(1)-1)*max(detrendEMD(:,1));
    detrThreshold(1) = quantileThreshold(1);
end
if quantileThreshold(2)<=1
    detrThreshold(2) = quantile(rmoutliers(detrendEMD(:,2)),quantileThreshold(2));
else%if quantileThreshold(2)<=2
    detrThreshold(2) = quantileThreshold(2);
    %detrThreshold(2) = (quantileThreshold(2)-1)*max(detrendEMD(:,2));
end
% disp(strcat("Up threshold: ",string(detrThreshold(1)),...
%     "|  Down threshold: ",string(detrThreshold(2))))

for curCrop = 1:cropCount

    switch baseFrameSide
        case 2 % base side - DOWN
            
            % Get Down-peak
            downPeakLoc = locs{2}(curCrop);
 
            % Find Up-peak
            upPeakLoc = locs{1}(find(abs(downPeakLoc -...
                locs{1})< UpDownKoef*frameLimit, 1,"first"));
            % If peak dont found - pass
            if isempty(upPeakLoc)
                continue;
            end

        case 1 % base side - UP
            % Get Up-peak
            upPeakLoc = locs{1}(curCrop);

            % Find Down-peak
            downPeakLoc = locs{2}(find(abs(upPeakLoc -...
                locs{2})< UpDownKoef*frameLimit, 1,"last"));
            % If peak dont found - pass
            if isempty(downPeakLoc)
                continue;
            end

        otherwise
            disp("Error! Wrong value of baseFrameSide");
            continue;
    end

    
    % FIND END FRAME
    
    endFrame = findEndFrame(detrendEMD, pks, frameLimit,...
                framesCount, detrThreshold(2), diflocs, curCrop, downPeakLoc);
    selectedFrames(curCrop,2) = endFrame;
    % FIND START FRAME
    startFrame = findStartFrame(detrendEMD, pks, frameLimit,...
                detrThreshold(1), framesCount, diflocs, curCrop, upPeakLoc);
    selectedFrames(curCrop,1) = startFrame;
    
end

% !!!Delete 0-0 rows
filtSelectedFrames = selectedFrames(any(selectedFrames>0,2),:);

%EMD, smoothedEMD, detrendEMD
returnEMD = detrendEMD;

% FOR CHECKING
% figure();
% tiledlayout(4,1);
% for side = 1:2
%     nexttile;
%     findpeaks(detrendEMD(:,side),...
%         analyzedFrames,'Annotate', 'extents','MinPeakDistance',frameLimit,...
%         'MinPeakHeight',quantile(detrendEMD(:,side),...
%         maxEMDQuantileValue(side)));
%     nexttile;
%     findpeaks(secDerEMD(:,side), analyzedFrames(3:end), ...
%         'Annotate', 'extents', 'MinPeakHeight', ...
%         quantile(secDerEMD(:,side), maxSecDifQuantileValue(side)));
% end



end
