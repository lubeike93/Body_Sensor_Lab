function [ feature_vec ] = generate_feature_vec( triaxial, svm )
%#codegen
%GENERATE_FEATURE_VEC extracts feature values from data sequence

meanSVMAcc = mean(svm); % Mean svm acceleration
meanXAcc = mean(triaxial(:, 1)); % Mean x acceleration
meanYAcc = mean(triaxial(: ,2)); % Mean y acceleration
meanZAcc = mean(triaxial(:, 3)); % Mean z acceleration

maxSVMAcc = max(svm);
maxXAcc = max(triaxial(:, 1));
maxYAcc = max(triaxial(:, 2));
maxZAcc = max(triaxial(:, 3));

minSVMAcc = min(svm);
minXAcc = min(triaxial(:, 1));
minYAcc = min(triaxial(:, 2));
minZAcc = min(triaxial(:, 3));

stdSVMAcc = std(svm);
stdXAcc = std(triaxial(:, 1));
stdYAcc = std(triaxial(:, 2));
stdZAcc = std(triaxial(:, 3));

feature_vec = [meanSVMAcc meanXAcc meanYAcc meanZAcc maxSVMAcc maxXAcc maxYAcc maxZAcc minSVMAcc minXAcc minYAcc minZAcc stdSVMAcc stdXAcc stdYAcc stdZAcc];
%feature_vec = feature_vec';
end

