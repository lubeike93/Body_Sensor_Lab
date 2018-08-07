p = load('parcour1_walking_running_jumping.txt');
walking = p(3748:6281, :);
running = p(10470:11670, :);
jumping = p(18710:19770, :);
[h_a, v_a] = acc_3dto2d(running);
% scatter(1:max(size(h_a)), h_a, 'filled')

peaks = findpeaks(v_a, 'MinPeakDistance', 38);
