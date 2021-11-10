function analyzedFrames = analyzeFrames(allVideos, template,...
    numRange, threshold, closeRadius, minPixels)
%ANALYZEFRAMES return cell of structs with img, binImg, properties of each frame
framesCount = size(allVideos{numRange},4);
analyzedFrames = cell(framesCount,1);

% relative intensity
relThresh = threshold/256;

% Morphological structuring element - disk
% disk mask decomposition
decomposition = 8;
se = strel('disk', closeRadius, decomposition);



% get frames numbers and images & Analyze images
for relFrameNum = 1:framesCount
    % current abs frame number
    analyzedFrames{relFrameNum}.frameNum = relFrameNum;
    % get image
    analyzedFrames{relFrameNum}.img  = im2gray(allVideos{numRange}(:,...
        :,:,relFrameNum));
    % create binary image of difference with template
    difImg = imabsdiff(analyzedFrames{relFrameNum}.img,template);
    binImg = imbinarize(difImg,relThresh);
    
    % cut up template up-rows
    %binImg(1:lastTemplRow,:) = 0;

    % close mask with disk
    binImg = imclose(binImg, se);    
    % delete objects with low count of pixels
    binImg = bwareaopen(binImg, minPixels);
    % fill holes
    binImg = imfill(binImg, 'holes');
    
    % Save binImg
    analyzedFrames{relFrameNum}.binImg = binImg;
  
    % Get regions and count of regions
    [L, numRegions] = bwlabel(binImg);
    % Save dropCount
    analyzedFrames{relFrameNum}.dropCount = numRegions;
    
    allDropImg = cell(numRegions,1);
    
    area = zeros(numRegions,1);
  
    %ROWS: DROPLETS
    %COLUMNS: 1 - rows (Y), 2 - columns (X)
    massCenter = zeros(numRegions,2);
    
    % Get each droplet
    for regNum = 1:numRegions
        % Get it's mask
        allDropImg{regNum} = false(size(binImg,1),size(binImg,2));
        allDropImg{regNum}(L == regNum) = true;
        % Get droplets characteristics
        area(regNum) = sum(allDropImg{regNum},'all');
        % droplet sums
        dropSumRows = zeros(size(binImg,1),1);
        dropSumColumns = zeros(size(binImg,2),1);
        % Get mass centers of droplets
        curImg = allDropImg{regNum};
        parfor i=1:size(binImg,1)
            dropSumRows(i) = i*sum(curImg(i,:),'all');
        end
        
        parfor j = 1:size(binImg,2)
            dropSumColumns(j) = j*sum(curImg(:,j),'all');
        end

        % Mass center
        massCenter(regNum,1) = sum(dropSumRows,'all')/area(regNum);
        massCenter(regNum,2) = sum(dropSumColumns,'all')/area(regNum); 
    end
  
    % get equivDiameter
    equivDiameter = sqrt(area.*(4/pi));
    
    % Save droplet characteristics
    analyzedFrames{relFrameNum}.massCenter = massCenter;
    analyzedFrames{relFrameNum}.area = area;
    analyzedFrames{relFrameNum}.diameter = equivDiameter;
    analyzedFrames{relFrameNum}.allDropImg = allDropImg;
end

