<?php

/**
 * @file
 * Implements token hooks.
 */

/**
 * Implements hook_token_info().
 */
function login_notify_token_info() {
  return array(
    'types' => array(
      'browser' => array(
        'name' => t('Browser'),
        'description' => t("The user's registered browser."),
        'needs-data' => 'browser',
      ),
    ),
    'tokens' => array(
      'browser' => array(
        'name' => array(
          'name' => t('Name'),
          'description' => t('The unique name assigned to the browser.'),
        ),
        'ip-address' => array(
          'name' => t('IP Address'),
          'description' => t('The IP address that registered the browser.'),
        ),
        'user-agent' => array(
          'name' => t('User Agent'),
          'description' => t('The user agent that registered the browser.'),
        ),
        'lock-url' => array(
          'name' => t('Lock URL'),
          'description' => t('The URL to lock the browser.'),
        ),
        'unlock-url' => array(
          'name' => t('Unock URL'),
          'description' => t('The URL to unlock the browser.'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function login_notify_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'browser' && !empty($data['browser'])) {
    $browser = $data['browser'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'name':
          $replacements[$original] = $sanitize ? check_plain($browser['name']) : $browser['name'];
          break;

        case 'ip-address':
          $replacements[$original] = $sanitize ? check_plain($browser['ip_address']) : $browser['ip_address'];
          break;

        case 'user-agent':
          $replacements[$original] = $sanitize ? check_plain($browser['user_agent']) : $browser['user_agent'];
          break;

        case 'lock-url':
          $url = url('user/' . $browser['uid'] . '/browsers/browser/' . $browser['browser_id'] . '/lock', array('absolute' => TRUE));
          $replacements[$original] = $sanitize ? check_plain($url) : $url;
          break;

        case 'unlock-url':
          $account = user_load($browser['uid']);
          $url = login_notify_browser_unlock_url($account, $browser);
          $replacements[$original] = $sanitize ? check_plain($url) : $url;
          break;
      }
    }
  }

  return $replacements;
}
