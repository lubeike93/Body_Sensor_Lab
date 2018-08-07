function [ svm ] = triaxial2svm( triaxial )
%TRIAXIAL2SVM Convert xyz-acceleration to single vector magnitude
svm = sqrt(triaxial(:, 1).^2 + triaxial(:, 2).^2 + triaxial(:, 3).^2);
end

