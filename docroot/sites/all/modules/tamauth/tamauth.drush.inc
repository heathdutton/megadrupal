<?php
/**
 * @file
 *
 * TAM Authenication drush functions.
 */


/**
 * Implements hook_drush_command().
 */
function tamauth_drush_command() {
  $items['tamauth'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('TAM Authentication'),
    'arguments' => array(
      'name' => dt('Username to use for authentication.'),
      'groups' => dt('Comma separated list of groups user belongs to.'),
    ),
    'options' => array(
      'name' => 'username',
    ),
  );
  return $items;
}

/**
 * Callback for tamauth command.
 */
function drush_tamauth($user, $groups) {
  global $base_url;

  $headers = array(
    "IV-USER: $user",
    "IV-GROUPS: $groups",
  );

  $path = $base_url . '/user/tam';
  $curl_options = array(
    CURLOPT_URL => $path,
    CURLOPT_HTTPHEADER => $headers,
    CURLOPT_RETURNTRANSFER => TRUE,
    CURLOPT_HEADER => TRUE,
    CURLINFO_HEADER_OUT => TRUE,
  );

  $ch = curl_init();
  curl_setopt_array($ch, $curl_options);
  $response = curl_exec($ch);
  $info = curl_getinfo($ch);
  curl_close($ch);

  if (200 !== $info['http_code'] && 302 !== $info['http_code']) {
    return drush_set_error(dt('Invalid reponse from server: @code', array('@code' => $info['http_code'])));
  }

  preg_match_all('/Set-Cookie: (.*)\b/', $response, $cookies);
  if (!isset($cookies[1][0])) {
    return drush_set_error(dt('Cookie not found'));
  }

  drush_print($cookies[1][0]);
}

/**
 * Get the settings from the cli
 *
 * @param
 *  String - Name
 * @return
 *  The setting or NULL
 */
function tamauth_get_option($option) {
  $options = drush_get_option_list($option);
  return !empty($options) ? $options[0] : NULL;
}
