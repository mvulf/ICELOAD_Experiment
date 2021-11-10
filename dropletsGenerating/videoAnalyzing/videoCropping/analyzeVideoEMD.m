function [analyzedFrames, EMD, histArray, imArray, imDifArray] = ...
    analyzeVideoEMD(obj, video, varargin)
%CROPVIDEO import video and analyze to finding onehistEMD
%   frameRange necessary if not whole video is analyzing
%   backgroundFrameCount - number of frames without droplets
%   return EMD and corrected frame array(from the beginning of whole video)
%   also return videofile and it's characteristics
%----------------------------------------------------
% overloading defaults values
defaults = {[1 1], 50, 30, 50, 2.0, 0.5};
defaults(1:nargin-2) = varargin;
[frameRange, threshold, upScaleKoef, downScaleKoef] = defaults{:};

% chosing frame count
framesCount = frameRange(2) + 1 - frameRange(1);

% full sizes
height = obj.Height;
width = obj.Width;

% NEW GETTING PARTS 
% [height width] of part-picture
% cell - whole, up, down
partSizes = {[height width], [floor(height/2) width],...
    [floor(height/2) width]};

% imgCropRange{i} = [xmin ymin width height] for imcrop(Im,imgCropRange{i})
% cell - whole, up, down
imgCropRange = {[1 1 flip(partSizes{1})],[1 1 flip(partSizes{2})],...
    [1 partSizes{2}(1)+1 flip(partSizes{3})]};

% initiate cell of arrays of images and difference  - whole, up, down
imArray = cell(1,3);
imDifArray = cell(1,3);

% creating EMD array - whole, up, down
EMD = zeros(framesCount,3);

% initiate array of hist values - whole, up, down
histArray = zeros(256,framesCount,3); 

% Get template
%medianTemplateImg is
templateImg = uint8(im2gray(median((video(:,:,:,:)),4)));

% Calculate EMD for all frames
for frameNum=1:framesCount
    % get frame
    frame = im2gray(video(:,:,:,frameNum));
    % get frame differ with template
    frameDiff = imabsdiff(frame,templateImg);
    % Parallel calculating of EMD values
    for i = 1:3
        imArray{i}(:,:,frameNum) = imcrop(frame,imgCropRange{i});
        imDifArray{i}(:,:,frameNum) = imcrop(frameDiff,imgCropRange{i});
        histArray(:,frameNum,i) = normImgHist(imDifArray{i}(:,:,frameNum));
        EMD(frameNum,i) = findOneHistEMD(histArray(:,frameNum,i),...
            threshold, upScaleKoef, downScaleKoef);
    end
end

% return analyzed frames
analyzedFrames = frameRange(1):frameRange(2);


end

