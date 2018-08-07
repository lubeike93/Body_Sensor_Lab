%#codegen
function [ feature_array ] = get_feature_array( triaxial_sequence )
%GET_FEATURE_ARRAY matlab implementation for code generation

gaussian_smoothed = triaxial_sequence;
numSeq = size(triaxial_sequence, 1);
sigma = 4;

% apply Gaussian onto xyz_sequence
for i = 1:3
    gaussian_smoothed(:, i) = gaussfilt(1:numSeq, triaxial_sequence(:, i), sigma);
end

training_svm = triaxial2svm(gaussian_smoothed);
smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, sigma);

abs_mean = (max(smoothed_svm) + min(smoothed_svm)) / 2;
mph = abs_mean + (max(smoothed_svm) - abs_mean) / 2;
mean_peaks = mean(findpeaks(smoothed_svm, 'MinPeakDistance', 35, 'MinPeakHeight', mph));

% extract feature values
feature_vector = generate_feature_vec(gaussian_smoothed, smoothed_svm);
feature_array = [feature_vector mean_peaks'];

end

