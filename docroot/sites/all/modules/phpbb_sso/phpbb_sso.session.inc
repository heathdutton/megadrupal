<?php

function _phpbb_sso_create_phpbb_session($phpbb_user) {
    if (!($sessions_table = phpbb_sso_get_table('sessions')))
        return;

    module_load_include('inc', 'phpbb_sso', 'phpbb_sso.utility');

    $now = time();
    $values = array(
        ':session_id' => md5(phpbb_sso_unique_id()),
        ':session_user_id' => $phpbb_user->user_id,
        ':session_forum_id' => 0,
        ':session_last_visit' => $now,
        ':session_start' => $now,
        ':session_time' => $now,
        ':session_ip' => phpbb_sso_get_client_ip(),
        ':session_browser' => (!empty($_SERVER['HTTP_USER_AGENT'])) ? htmlspecialchars((string) $_SERVER['HTTP_USER_AGENT']) : '',
        ':session_forwarded_for' => (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) ? htmlspecialchars((string) $_SERVER['HTTP_X_FORWARDED_FOR']) : '',
        ':session_page' => 'index.php',
        ':session_viewonline' => 1,
        ':session_autologin' => 0,
        ':session_admin' => 0, // todo
    );

    db_query("DELETE FROM {$sessions_table} WHERE session_user_id = :user_id OR session_user_id = :anonymous", array(':user_id' => $phpbb_user->user_id, ':anonymous' => 1));
    db_query("INSERT INTO {$sessions_table} (session_id, session_user_id, session_forum_id, session_last_visit, session_start, session_time, session_ip, session_browser, session_forwarded_for, session_page, session_viewonline, session_autologin, session_admin) VALUES (:session_id, :session_user_id, :session_forum_id, :session_last_visit, :session_start, :session_time, :session_ip, :session_browser, :session_forwarded_for, :session_page, :session_viewonline, :session_autologin, :session_admin)", $values);

    $cookie_expire = $now + 31536000; // expires in 365 days

    phpbb_sso_set_cookie('u', $phpbb_user->user_id, $cookie_expire);
    phpbb_sso_set_cookie('k', '', $cookie_expire);
    phpbb_sso_set_cookie('sid', $values[':session_id'], $cookie_expire);

    return true;
}

function _phpbb_sso_destroy_phpbb_session($phpbb_user) {
    if (!($sessions_table = phpbb_sso_get_table('sessions')))
        return;

    module_load_include('inc', 'phpbb_sso', 'phpbb_sso.utility');
    
    db_query("DELETE FROM {$sessions_table} WHERE session_user_id = :user_id OR session_user_id = :anonymous", array(':user_id' => $phpbb_user->user_id, ':anonymous' => 1));

    phpbb_sso_unset_cookie('u');
    phpbb_sso_unset_cookie('k');
    phpbb_sso_unset_cookie('sid');
    
    return true;
}

/**
 * Sets a cookie
 *
 * Sets a cookie of the given name with the specified data for the given length of time. If no time is specified, a session cookie will be set.
 *
 * @param string $name     Name of the cookie, will be automatically prefixed with the phpBB cookie name. track becomes [cookie_name]_track then.
 * @param string $data     The data to hold within the cookie
 * @param int $time        The expiration time as UNIX timestamp. If 0 is provided, a session cookie is set.
 */
function phpbb_sso_set_cookie($name, $data, $time) {
    $config = phpbb_sso_get_config(array('cookie_name', 'cookie_path', 'cookie_domain', 'cookie_secure'));

    $name = rawurlencode($config['cookie_name'] . '_' . $name) . '=' . rawurlencode($data);
    $expire = gmdate('D, d-M-Y H:i:s \\G\\M\\T', $time);
    $domain = (empty($config['cookie_domain']) || $config['cookie_domain'] == 'localhost' || $config['cookie_domain'] == '127.0.0.1') ? '' : '; domain=' . $config['cookie_domain'];

    header('Set-Cookie: ' . $name . (($time) ? '; expires=' . $expire : '')
         . '; path=' . $config['cookie_path'] . $domain . ((empty($config['cookie_secure'])) ? '' : '; secure') . '; HttpOnly', false);
}

function phpbb_sso_unset_cookie($name) {
    $expire = time() - 3600;
    header('Set-Cookie: ' . check_plain($name) . '; expires=' . $expire, false);
}

function phpbb_sso_get_client_ip() {
    $ip = (!empty($_SERVER['REMOTE_ADDR'])) ? (string) $_SERVER['REMOTE_ADDR'] : '';
    $ip = preg_replace('# {2,}#', ' ', str_replace(',', ' ', $ip));

    // split the list of IPs
    $ips = explode(' ', trim($ip));

    // Default IP if REMOTE_ADDR is invalid
    $ip = '127.0.0.1';

    foreach ($ips as $ip) {
        if (preg_match('#^(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$#', $ip)) {
            $ip = $ip;
        }
        else if (preg_match('#^(?:(?:(?:[\dA-F]{1,4}:){6}(?:[\dA-F]{1,4}:[\dA-F]{1,4}|(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])))|(?:::(?:[\dA-F]{1,4}:){0,5}(?:[\dA-F]{1,4}(?::[\dA-F]{1,4})?|(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])))|(?:(?:[\dA-F]{1,4}:):(?:[\dA-F]{1,4}:){4}(?:[\dA-F]{1,4}:[\dA-F]{1,4}|(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])))|(?:(?:[\dA-F]{1,4}:){1,2}:(?:[\dA-F]{1,4}:){3}(?:[\dA-F]{1,4}:[\dA-F]{1,4}|(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])))|(?:(?:[\dA-F]{1,4}:){1,3}:(?:[\dA-F]{1,4}:){2}(?:[\dA-F]{1,4}:[\dA-F]{1,4}|(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])))|(?:(?:[\dA-F]{1,4}:){1,4}:(?:[\dA-F]{1,4}:)(?:[\dA-F]{1,4}:[\dA-F]{1,4}|(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])))|(?:(?:[\dA-F]{1,4}:){1,5}:(?:[\dA-F]{1,4}:[\dA-F]{1,4}|(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])))|(?:(?:[\dA-F]{1,4}:){1,6}:[\dA-F]{1,4})|(?:(?:[\dA-F]{1,4}:){1,7}:)|(?:::))$#i', $ip))
        {
            // Quick check for IPv4-mapped address in IPv6
            if (stripos($ip, '::ffff:') === 0) {
                $ipv4 = substr($ip, 7);

                if (preg_match('#^(?:(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$#', $ipv4)) {
                    $ip = $ipv4;
                }
            }

            $ip = $ip;
        }
        else {
            // We want to use the last valid address in the chain
            // Leave foreach loop when address is invalid
            break;
        }
    }

    return $ip;
}
