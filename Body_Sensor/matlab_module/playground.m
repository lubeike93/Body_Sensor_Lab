p = load('parcour1_walking_running_jumping.txt');
walking = p(3748:6281, :);
running = p(10470:11670, :);
jumping = p(18710:19770, :);

walking_num = get_step_num(walking, 0);
running_num = get_step_num(running, 1);
jumping_num = get_step_num(jumping, 2);

%%
nowtry = walking;
sigma = 4;

gaussian_smoothed = nowtry(:, :); % First column contains label.
numSeq = size(nowtry, 1);

for i = 1:3
    gaussian_smoothed(:, i) = gaussfilt(1:numSeq, nowtry(:, i), sigma);
end

training_svm = triaxial2svm(gaussian_smoothed);
smoothed_svm = gaussfilt(1:size(training_svm, 1), training_svm, sigma);
%figure,plot(walking)
figure('Name', 'smoothed'),plot(smoothed_svm)

% Assuming Ts = 8 ms
%figure, findpeaks(smoothed_svm, 'MinPeakDistance', peak_min_dis)
mph = mean(smoothed_svm) + ((max(smoothed_svm) - mean(smoothed_svm)) / 2);
figure,findpeaks(smoothed_svm, 'MinPeakDistance', 40, 'MinPeakHeight', mph)
% mean_peaks = mean(peaks);

%% api test
[walk_train_data, walk_test_data, walk_num_train, ~] = data_slicer(walking, 128, 1);
[run_train_data, run_test_data, run_num_train, ~] = data_slicer(running, 128, 2);
[jump_train_data, jump_test_data, jump_num_train, ~] = data_slicer(jumping, 128, 3);
endnum = 0;

for i = 1:jump_num_train
    [labels(i), num_steps] = predict_tree(jump_train_data(:,2:end, i));
    endnum = endnum + num_steps;
end
endnum

