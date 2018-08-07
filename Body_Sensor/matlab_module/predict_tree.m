function [ label, num_steps ] = predict_tree( triaxial_sequence )
%#codegen
%PREDICT_TREE the api
S = coder.load('tree_171214.mat');

gaussian_smoothed = triaxial_sequence;
numSeq = size(triaxial_sequence, 1);
sigma = 4;

for i = 1:3
    gaussian_smoothed(:, i) = gaussfilt(1:numSeq, triaxial_sequence(:, i), sigma);
end

training_svm = triaxial2svm(gaussian_smoothed);
smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, sigma);

% Assuming Ts = 8 ms
abs_mean = (max(smoothed_svm) + min(smoothed_svm)) / 2;
mph = abs_mean + (max(smoothed_svm) - abs_mean) / 2;
mean_peaks = mean(findpeaks(smoothed_svm, 'MinPeakDistance', 35, 'MinPeakHeight', mph));

feature_vector = generate_feature_vec(gaussian_smoothed, training_svm);
feature_vector = [feature_vector mean_peaks'];

label = predict(S.tree, feature_vector);

switch label
    case 1
        smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, 4);
        mph = mean(smoothed_svm);
        peaks = findpeaks(smoothed_svm, 'MinPeakDistance', 40, 'MinPeakHeight', mph);
        num_steps = max(size(peaks));
    case 2
        smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, 8);
        mph = mean(smoothed_svm);% + ((max(smoothed_svm) - mean(smoothed_svm)) / 2);
        peaks = findpeaks(smoothed_svm, 'MinPeakDistance', 25, 'MinPeakHeight', mph);
        num_steps = max(size(peaks));
    case 3
        smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, 10);
        mph = mean(smoothed_svm) + ((max(smoothed_svm) - mean(smoothed_svm)) / 2);
        peaks = findpeaks(smoothed_svm, 'MinPeakDistance', 40, 'MinPeakHeight', mph);
        num_steps = max(size(peaks));
    otherwise
        num_steps = 0;
end

end

