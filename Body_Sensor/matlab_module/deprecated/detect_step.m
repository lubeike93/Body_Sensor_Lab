function [ maxCount, tempMax, tempMin ] = detect_step( x, y )
%DETECT_STEP Summary of this function goes here
%   Detailed explanation goes here
% [x] = ms

maxCount = 0;
minCount = 0;
tempMax = 0.0;
tempMin = mean(y);
tMax = 0;
tMin = 0;
sumMax = 0.0;
sumMin = 0.0;

MMax = y(1);
MMaxPos = x(1);
MMin = y(1);
MMinPos = x(1);

numData = max(size(x));
flag = 0; % 0 for bottom, 1 for peak
i = 1;

while i < numData
    i = i+1;
    if y(i) > MMax
        MMax = y(i);
        MMaxPos = x(i);
    elseif y(i) < MMin
        MMin = y(i);
        MMinPos = x(i);
    end
    
    if MMaxPos - tMax > 300 && flag == 1
        sumMax = sumMax + MMax;
        tMax = MMaxPos;
        tempMax = MMax;
        maxCount = maxCount + 1;
        flag = 0;
    elseif MMax > tempMax
        tempMax = MMax;
        tMax = MMaxPos;
    end
    
    if MMinPos - tMin > 300 && flag == 0
        minCount = minCount + 1;
        sumMin = sumMin + tempMin;
        flag = 1;
        tMin = MMinPos;
        tempMin = MMin;
    elseif MMin < tempMin
        tempMin = MMin;
        tMin = MMinPos;
    end
end
    
end

