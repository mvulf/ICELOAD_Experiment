function cropRanges = cropRanges2str(selectedFrames)
%CROPRANGES2STR Converts array of selectedFrames to str
strFrame = strings(1,2);
cropCount = size(selectedFrames,1);
strArr = strings(1, cropCount);
for i=1:cropCount
    % replace '0' -> ''
    strFrame(:) = string(selectedFrames(i,:));
    strFrame(strFrame == '0') = '';

    strArr(i) = append(strFrame(1),'-',strFrame(2));
end
cropRanges = sprintf('%s,',strArr);
cropRanges = extractBefore(cropRanges,strlength(cropRanges));

end

