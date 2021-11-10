function [allVideos, template, videoCount] = loadVideos(testCount,...
    dataCell, videoFolderPath, croppingFolderPath)
allVideos = cell(testCount,1);
template = cell(testCount,1);
testNum = zeros(testCount,1);
videoCount = zeros(testCount,1);

parfor relTestNum = 1:testCount
    strTestNum = dataCell{relTestNum};
    testNum(relTestNum) = uint16(str2double(strTestNum));
    
    % CREATE VIDEO TABLE
    videoT = struct2table(dir(strcat(videoFolderPath,...
        strTestNum,'_photron_*.avi')));
    videoCount(relTestNum) = height(videoT);
    disp(strcat(string(relTestNum),". Test #", strTestNum));
    disp(strcat("Count of videos: ", string(videoCount(relTestNum))));
    
    % LOAD Template
    templateName = strcat(strTestNum,"_template.jpg");
    templatePath = strcat(croppingFolderPath, templateName);
    if exist(templatePath,'file')    
        template{relTestNum} = imread(templatePath);
        disp(strcat(templateName," loaded"));
    else
        disp("Error! Template image not found.")
    end

    % LOAD VIDEOS
    allVideos{relTestNum} = cell(videoCount(relTestNum),1);
    for numRange = 1:videoCount(relTestNum)
        if videoCount(relTestNum) > 1
            videoPath = string(strcat(videoT.folder(numRange),...
                '\', videoT.name(numRange)));
        else
            videoPath = string(strcat(videoT.folder, '\', videoT.name));
        end
        obj = VideoReader(videoPath);
        disp(strcat(string(numRange), ") loading..."));
        allVideos{relTestNum}{numRange} = obj.read();
        disp("...loaded");
    end

end
end