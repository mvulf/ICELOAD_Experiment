function [propT, dropDiameter, fitYvelocity] =...
    postAnalyzeFrames(analyzedFrames, estimDropCount, fig, figTitle)

% ANALYZING IN SINGLE CASE 
% !!! IN MULTI - create propetry table for each droplet (under PARFOR)
propT = table();
dropDiameter = zeros(1,2);
fitYvelocity = zeros(2,1);
if estimDropCount == 1
    % Get first frame, when count of droplets increased
    firstRelFrame = 1;
    for frame = 1:fix(0.75*length(analyzedFrames))
        if analyzedFrames{frame}.dropCount >= 2
            firstRelFrame = frame;
            break
        end
    end
    disp(strcat("First relative frame number, when droplet exists: ",...
        sprintf("%d",firstRelFrame)));

    % Get last frame after wich, only needle region exist
    lastRelFrame = length(analyzedFrames);
    for frame = flip(firstRelFrame:lastRelFrame)
        if analyzedFrames{frame}.dropCount == 2
            lastRelFrame = frame;
            break
        end
    end
    disp(strcat("Last relative frame number, when droplet exists: ",...
        sprintf("%d",lastRelFrame)));
    
    % Create Vars
    frameNum = zeros(length(analyzedFrames),1);
    index = uint8(zeros(length(analyzedFrames),1));
    y = zeros(length(analyzedFrames),1);
    x = zeros(length(analyzedFrames),1);
    diameter = zeros(length(analyzedFrames),1);
    area = zeros(length(analyzedFrames),1);
    
    % Retrieve DOWN droplets characteristics
    parfor frame = firstRelFrame:lastRelFrame %parfor
        if analyzedFrames{frame}.dropCount > 0
            frameNum(frame) = analyzedFrames{frame}.frameNum;
            % !!! Get index of row with down droplet and coordinates
            [y(frame),index(frame)] =...
                max(analyzedFrames{frame}.massCenter(:,1));
            x(frame) = analyzedFrames{frame}.massCenter(index(frame),2);
            diameter(frame) =...
                analyzedFrames{frame}.diameter(index(frame),1);
            area(frame) = analyzedFrames{frame}.area(index(frame),1);
        end 
    end
    % Create table
    propT = table(frameNum,y,x,diameter,area);
    propT(~propT.frameNum,:) = [];
    % Remove outliers by Area
    [~,TF] = rmoutliers(propT.area);
    propT(TF,:) = [];
    
    % Get diameter
    [dropDiameter(1),dropDiameter(2)] = normfit(propT.diameter);
    
    % Y-INTERPOLATION 
    fitY = fit(propT.frameNum,propT.y, 'poly3','Normalize','on');
    % Get velocity by differentiation
    fitYvelocity(1) = differentiate(fitY,propT.frameNum(1));
    fitYvelocity(2) = differentiate(fitY,propT.frameNum(end));

    disp(strcat("Average droplet's diameter, pixels: ",...
        sprintf("%0.1f",dropDiameter(1))));
    disp(strcat("Sigma of droplet's diameter, pixels: ",...
        sprintf("%0.1f",dropDiameter(2))));
    disp(strcat("Starting velocity, pixels per frame: ",...
        sprintf("%0.1f",fitYvelocity(1))));
    disp(strcat("Ending velocity, pixels per frame: ",...
        sprintf("%0.1f",fitYvelocity(2))));
    
    % Get x range
    xMin = min(propT.x);
    xMax = max(propT.x);
    xDelta = xMax - xMin;

    % Plot XY-graphic
    t = tiledlayout(fig, 2,3);
    %title(t,figTitle);

    % set(gcf, 'NumberTitle', 'off', 'Name', figureName);
    nexttile;
    pl(1) = plot([0,0],[-max(propT.y),-min(propT.y)],'--b','DisplayName', ...
        'vertical');
    hold on
    axis([-(xDelta+5) +(xDelta+5) -max(propT.y) -min(propT.y)]);
    pl(2) = plot(propT.x - propT.x(1),-propT.y,'.r', 'DisplayName', ...
        'Experimental');
    title({figTitle,"Droplet mass centres"});
    xlabel("\DeltaX, pixels");
    ylabel("Y, pixels");
    %l = legend(["vertical","Experimental"]);
    hold off
    nexttile;
    % Plot Y-graphic
    plot(propT.frameNum,-propT.y,'.r');
    % get frames range
    frameFit = propT.frameNum(1):propT.frameNum(end);
    hold on
    pl(3) = plot(frameFit,-fitY(frameFit),'-b', 'DisplayName',...
        'Fit curve');
    axis([propT.frameNum(1) propT.frameNum(end) -max(propT.y) ...
        -min(propT.y)]);
    title("Vertical displacement");
    xlabel("Frame number");
    ylabel("Y, pixels");
    %legend([l,"Experimental","Fit curve"],"Location","southwest");
    hold off
    % VELOCITY
    nexttile;
    velocity = differentiate(fitY,frameFit);
    plot(frameFit,velocity,'-b');
    axis([propT.frameNum(1) propT.frameNum(end) velocity(1) ...
        velocity(end)]);
    title("Vertical velocity");
    xlabel("Frame number");
    ylabel("-V_y, ^{pixels}/_{frame}");


    % DISPLAY START-END ANALYZING FRAMES
    nexttile;
    absFrame = propT.frameNum(1);
    relFrameNum = absFrame + 1 - analyzedFrames{1}.frameNum;
    montage({analyzedFrames{relFrameNum}.allDropImg{index(relFrameNum)},...
        analyzedFrames{relFrameNum}.img});
    title(strcat("First frame #",string(absFrame)));
    
    nexttile;
    absFrame = propT.frameNum(fix(end/2));   
    relFrameNum = absFrame + 1 - analyzedFrames{1}.frameNum;
    montage({analyzedFrames{relFrameNum}.allDropImg{index(relFrameNum)},...
        analyzedFrames{relFrameNum}.img});
    title(strcat("Middle frame #",string(absFrame)));

    nexttile;
    absFrame = propT.frameNum(end);
    relFrameNum = absFrame + 1 - analyzedFrames{1}.frameNum;
    montage({analyzedFrames{relFrameNum}.allDropImg{index(relFrameNum)},...
        analyzedFrames{relFrameNum}.img});
    title(strcat("Last frame #",string(absFrame)));

    % set(gcf,'Name',figureName);
    leg = legend(pl, 'Orientation', 'horizontal');
    leg.Layout.Tile = 'south';
end
end