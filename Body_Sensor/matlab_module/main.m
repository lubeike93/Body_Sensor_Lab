%% Load time series
p = load('parcour1_walking_running_jumping.txt');
walking = p(3748:6281, :);
running = p(10470:11670, :);
jumping = p(18710:19770, :);

%%
walkingf = get_feature_array(walking(1:64, :));
runningf = get_feature_array(running(1:64, :));
jumpingf = get_feature_array(jumping(1:64, :));
together = [walkingf; runningf; jumpingf];

%% Split into train and test set (GENERATE TRAINING DATA!!!!!!!)
jumping = [jumping; modified_jump];
walking = [walking; modified_walk];
running = [running; modified_run];

%%
[walk_train_data, walk_test_data, walk_num_train, ~] = data_slicer(walking(1:end, :), 128, 0);
[run_train_data, run_test_data, run_num_train, ~] = data_slicer(running(1:end, :), 128, 1);
[jump_train_data, jump_test_data, jump_num_train, ~] = data_slicer(jumping(1:end, :), 128, 2);
%[sit_train_data, ~, sit_num_train, ~] = data_slicer(sit(861:end, :), 64, 3);
%[stand_train_data, ~, stand_num_train, ~] = data_slicer(stand_adv(1:end, :), 64, 4);

%% Generate feature matrices
walk_feature_mat = zeros(walk_num_train, 18);
for i = 1:walk_num_train
    walk_feature_mat(i, 1) = 0;
    walk_feature_mat(i, 2:end) = get_feature_array(walk_train_data(:,2:end,i));
end

run_feature_mat = zeros(run_num_train, 18);
for i = 1:run_num_train
    run_feature_mat(i, 1) = 1;
    run_feature_mat(i, 2:end) = get_feature_array(run_train_data(:,2:end,i));
end

jump_feature_mat = zeros(jump_num_train, 18);
for i = 1:jump_num_train
    jump_feature_mat(i, 1) = 2;
    jump_feature_mat(i, 2:end) = get_feature_array(jump_train_data(:,2:end,i));
end

big_mat = [walk_feature_mat; run_feature_mat; jump_feature_mat];

%% Check feature std, use 4 5 9 12 13 16
stddev = std(big_mod_mat(:, 2:end));
summ = sum(sum(stddev, 1));

contribution = zeros(1, 17);
for i = 1:17
    contribution(i) = stddev(i) / summ;
end

%% PCA does not work well
% [coeff, score, latent,tsquared,explained,mu] = pca(big_mat(:, 1:end-1)); % Do PCA
% princomps = coeff(:, 1:10); % Only use the first three PCs
% small_mat = [big_mat(:, 1:end-1)*princomps big_mat(:, end)];

%% Train a DT
tree = fitctree(big_mat(:, 2:end), big_mat(:, 1));

%% Train an NBC
nbc_mat = big_mat;
nbc_mat(:,9+1) = []; % 0 variance
nbc = fitcnb(nbc_mat(:, 2:end), nbc_mat(:, 1));


%% Testing
[walk_test_data, ~, walk_num_test, ~] = data_slicer(walking(10:end, :), 100, 1);
[run_test_data, ~, run_num_test, ~] = data_slicer(running(8:end, :), 100, 2);
[jump_test_data, ~, jump_num_test, ~] = data_slicer(jumping(7:end, :), 100, 3);

for i = 1:walk_num_test
    [walk_test_mat(i, :)] = preprocess(walk_test_data(:,:,i), 30);
end
for i = 1:run_num_test
    [run_test_mat(i, :)] = preprocess(run_test_data(:,:,i), 30);
end
for i = 1:jump_num_test
    [jump_test_mat(i, :)] = preprocess(jump_test_data(:,:,i), 30);

end
% figure,plot(run_test_mat(36,:))
test_mat = [walk_test_mat; run_test_mat; jump_test_mat];
% for i = 1:size(test_mat, 1)
%     test_mat(i, :) = test_mat(i, :) - mu;
% end
% test_mat = test_mat * princomps;
% dt_labels = predict(tree, test_mat(:, 2:end));
% test_mat(:,9+1) = [];
% nb_labels = predict(nbc, test_mat(:, 2:end));


%% load new data

