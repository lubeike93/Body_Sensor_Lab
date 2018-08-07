[pathstr, name, ext] = fileparts(which(bdroot(gcs)));
path = [pathstr filesep name '_ert_rtw' filesep];
[~, cfiles] = system(['find ' path ' \( -name "*.c" \) \( ! -name "ert_main.c" \) -type f']);
[~, hfiles] = system(['find ' path ' \( -name "*.h" \) -type f']);
fileId = fopen('~/.simulinkfilecopy', 'w');
fprintf(fileId, '%s', [cfiles, hfiles]);
fclose(fileId);