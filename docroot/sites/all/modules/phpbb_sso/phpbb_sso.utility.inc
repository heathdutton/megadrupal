<?php

/**
 * Return unique id
 * @param string $extra additional entropy
 */
function phpbb_sso_unique_id($extra = 'c') {
    $config = phpbb_sso_get_config(array('rand_seed'));

    $val = $config['rand_seed'] . microtime();
    $val = md5($val);
    $rand_seed = md5($config['rand_seed'] . $val . $extra);

    return substr($val, 4, 16);
}

function phpbb_sso_get_phpbb_version() {
    $path = realpath('.') . '/' . variable_get('phpbb_sso_phpbb_path', '');
    $file = $path . '/includes/constants.php';
    if (!is_file($file)) {
        phpbb_sso_debug('Could not determine phpBB version: includes/constants.php file not found.');
        return;
    }

    $content = file_get_contents($path . '/includes/constants.php');
    if (preg_match("/define[(]'PHPBB_VERSION',\s*'([^']+)'[)]/", $content, $matches)) {
        return trim($matches[1]);
    } else {
        phpbb_sso_debug('Could not determine phpBB version: PHPBB_VERSION constant not found in includes/constants.php.');
    }
}
