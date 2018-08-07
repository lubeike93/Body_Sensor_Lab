function [ feature_vector ] = preprocess( training_sequence, peak_min_dis )
%#codegen
%PREPROCESS For given training data (a sequence), apply gaussian smoothing
%filter, convert triaxial to svm, return feature vector. First element of
%FV is label of the sequence (1:walking, 2:running, 3:jumping).

%% Apply Gaussian filter on time series
gaussian_smoothed = training_sequence(:, 2:end); % First column contains label.

numSeq = size(training_sequence, 1);
sigma = 4;

for i = 1:3
    gaussian_smoothed(:, i) = gaussfilt(1:numSeq, training_sequence(:, i+1), sigma);
end

%% Triaxial to Single Vector Magnitude (SVM) and find peaks
training_svm = triaxial2svm(gaussian_smoothed);
smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, sigma);

% Assuming Ts = 8 ms
%figure, findpeaks(smoothed_svm, 'MinPeakDistance', peak_min_dis)
abs_mean = (max(smoothed_svm) + min(smoothed_svm)) / 2;
mph = abs_mean + (max(smoothed_svm) - abs_mean) / 2;
mean_peaks = mean(findpeaks(smoothed_svm, 'MinPeakDistance', peak_min_dis, 'MinPeakHeight', mph));

%% Define features
feature_vector = generate_feature_vec(gaussian_smoothed, training_svm);
feature_vector = [training_sequence(1, 1) feature_vector mean_peaks'];

end

