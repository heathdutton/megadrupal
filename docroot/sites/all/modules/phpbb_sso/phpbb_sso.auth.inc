<?php

function _phpbb_sso_check_user($username, $password) {
    global $db, $config;

    if (!($users_table = phpbb_sso_get_table('users')))
        return;

    $results = db_query("SELECT user_id, user_password, user_type
                         FROM {$users_table}
                         WHERE username_clean = :username", array(':username' => phpbb_sso_utf8_clean_string($username)));

    if (!$results->rowCount())
        return false;

    module_load_include('inc', 'phpbb_sso', 'phpbb_sso.hash');
    $user_types = array('USER_NORMAL', 'USER_INACTIVE', 'USER_IGNORE', 'USER_FOUNDER');

    foreach ($results as $row) {
        $hash_manager = new phpbb_sso_hash_manager($row->user_password);
        if (!$hash_manager->check_hash($password, $row->user_password))
            continue;

        // Check for old password hash
        if (strlen($row->user_password) == 32) {
            $hash = $hash_manager->hash($password);

            // Update the password in the users table to the new format
            db_query("UPDATE {$users_table} SET user_password = :user_password, user_pass_convert = 0 WHERE user_id = :user_id", array(':user_password' => $hash, ':user_id' => $row->user_id));
        }

        // Is the account inactive?
        switch ($user_types[$row->user_type]) {
            case 'USER_INACTIVE':
            case 'USER_IGNORE':
                return false;
        }

        return true;
    }

    return false;
}

function _phpbb_sso_get_user($username) {
    if (!($users_table = phpbb_sso_get_table('users')))
        return;

    $result = db_query("SELECT *
                        FROM {$users_table}
                        WHERE username_clean = :username", array(':username' => phpbb_sso_utf8_clean_string($username)));

    if (!$result->rowCount())
        return false;

    $phpbb_user = $result->fetchObject();
    return $phpbb_user;
}

function phpbb_sso_utf8_clean_string($text) {
    module_load_include('inc', 'phpbb_sso', 'phpbb_sso.utf8');
    static $homographs = array();
    if (empty($homographs)) {
        module_load_include('inc', 'phpbb_sso', 'phpbb_sso.confusables');
        $homographs = phpbb_sso_get_homographs();
    }

    $text = utf8_case_fold_nfkc($text);
    $text = strtr($text, $homographs);
    // Other control characters
    $text = preg_replace('#(?:[\x00-\x1F\x7F]+|(?:\xC2[\x80-\x9F])+)#', '', $text);

    // we need to reduce multiple spaces to a single one
    $text = preg_replace('# {2,}#', ' ', $text);

    // we can use trim here as all the other space characters should have been turned
    // into normal ASCII spaces by now
    return trim($text);
}
