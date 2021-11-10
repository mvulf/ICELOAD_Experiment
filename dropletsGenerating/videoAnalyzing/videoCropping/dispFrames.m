function res = dispFrames(selectedFrames, analyzedFrames, EMD, ...
    video, testNum)
% selectedFrames - array of begin-end_range rows
% frameStruct.frameNum -> array (x,1)
% frameStruct.name -> str cell
% frameStruct.img -> cell

% PREPARING STRUCT
frameStruct.frameNum = [];
frameStruct.name = {};
frameStruct.img = {};

% if length(selectedFrames) == 0, "for" will not go inside
for cropNum = 1:1:size(selectedFrames,1)
    % if there are no zeros
    if size(selectedFrames,2) == 2
        if ~any(selectedFrames(cropNum,:) == 0)
            % frame numbers
            beginFrame = selectedFrames(cropNum,1);
            endFrame = selectedFrames(cropNum,2);
            middleFrame = floor((beginFrame+endFrame)/2);

            % getting begin-frame
            frameStruct.frameNum(end+1) = beginFrame;
            frameStruct.name{end+1,1} = strcat('№', string(cropNum),...
                ' begin #', string(frameStruct.frameNum(end)));
            frameStruct.img{end+1,1} = im2gray(video(:,:,:,...
                frameStruct.frameNum(end)));

            % getting middle-frame
            frameStruct.frameNum(end+1) = middleFrame;
            frameStruct.name{end,2} = strcat('middle #',...
               string(frameStruct.frameNum(end)));
            frameStruct.img{end,2} = im2gray(video(:,:,:,...
                frameStruct.frameNum(end)));

            % getting end-frame
            frameStruct.frameNum(end+1) = endFrame;
            frameStruct.name{end,3} = strcat('end #',...
               string(frameStruct.frameNum(end)));
            frameStruct.img{end,3} = im2gray(video(:,:,:,...
                frameStruct.frameNum(end)));
        %on case [frame 0] and [0 frame]. [0 0] is impossible
        else
            % getting one frame number
            % first frame is zero
            if selectedFrames(cropNum,2) == 0
                frameStruct.frameNum(end+1) = selectedFrames(cropNum,1);
                frameStruct.name{end+1,1} = strcat('Only begin (up) №',...
                    string(cropNum),' #',...
                    string(frameStruct.frameNum(end)));
            % last frame is zero
            else 
                frameStruct.frameNum(end+1) = selectedFrames(cropNum,2);
                frameStruct.name{end+1,1} = strcat('Only end (down) №',...
                    string(cropNum),' #',...
                    string(frameStruct.frameNum(end)));
            end
            
            % getting image by frameNum
            frameStruct.img{end+1,1} = im2gray(video(:,:,:,...
                frameStruct.frameNum(end)));
        end
    % in case of empty or incorrect cell                   
    else
       frameStruct.frameNum(end+1) = 0;
       disp(strcat("Error! Empty or incorrect selected frames.",... 
           "Cropping #", string(cropNum)));

    end
end

ymax = max(max(EMD))+0.05;
ymin = min(min(EMD))-0.05;

tiledlayout(ceil((size(selectedFrames,1)+1)/2),2,"TileSpacing","tight"); 
% "TileSpacing","compact"

nexttile(1, [1 1]);
plot(analyzedFrames,EMD(:,1), ...
    analyzedFrames,EMD(:,2),'Marker','.')

labels = cell(2,1);
labels{1} = "up EMD";
labels{2} = "down EMD";
    
% plot frames cropping (if no frames - go further)
for i = 1:1:length(frameStruct.frameNum)
    frameNum = frameStruct.frameNum(i);
    hold on
    plot([frameNum frameNum],[ymin ymax],'LineStyle',"--",'Marker',"none")
end

% Add xlabel, ylabel, title
xlabel('analyzedFrames');
ylabel('EMD');
title(strcat(sprintf('%04u',testNum),' EMD distribution'));
legend(labels,'Location','northeastoutside');

hold off;

% if no images - skip!
for i = 1:1:size(frameStruct.img,1)
    % if frame is exist
    if frameStruct.frameNum(i) ~= 0
        nexttile;
        % If there is ONE-image crop - display one frame
        if (size(frameStruct.img,2)==1)
            imshow(frameStruct.img{i,1})
            title(frameStruct.name{i,1})

        else
            montage({frameStruct.img{i,1}, frameStruct.img{i,2},...
                frameStruct.img{i,3}}, 'Size', [1 3]);
            title(strcat(frameStruct.name{i,1},{', '}, ...
                frameStruct.name{i,2},{', '},frameStruct.name{i,3}))
        end
    end
res = 0;

end


