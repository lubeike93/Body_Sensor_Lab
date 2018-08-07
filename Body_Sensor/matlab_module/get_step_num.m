%#codegen
function [ num_steps ] = get_step_num( triaxial_sequence, status )
%GET_STEP_NUM Summary of this function goes here
%   Detailed explanation goes here
% 0 for walking, 1 for running, 2 for jumping

gaussian_smoothed = triaxial_sequence;
numSeq = size(triaxial_sequence, 1);
sigma = 4;

for i = 1:3
    gaussian_smoothed(:, i) = gaussfilt(1:numSeq, triaxial_sequence(:, i), sigma);
end

training_svm = triaxial2svm(gaussian_smoothed);

switch status
    case 0
        smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, 4);
        mph = mean(smoothed_svm);
        peaks = findpeaks(smoothed_svm, 'MinPeakDistance', 40, 'MinPeakHeight', mph);
        num_steps = max(size(peaks));
    case 1
        smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, 8);
        mph = mean(smoothed_svm);% + ((max(smoothed_svm) - mean(smoothed_svm)) / 2);
        peaks = findpeaks(smoothed_svm, 'MinPeakDistance', 25, 'MinPeakHeight', mph);
        num_steps = max(size(peaks));
    case 2
        smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, 10);
        mph = mean(smoothed_svm) + ((max(smoothed_svm) - mean(smoothed_svm)) / 2);
        peaks = findpeaks(smoothed_svm, 'MinPeakDistance', 40, 'MinPeakHeight', mph);
        num_steps = max(size(peaks));
    otherwise
        num_steps = 0;
end

end

