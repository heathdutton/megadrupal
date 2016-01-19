<?php

/**
 * @file
 * Builds placeholder replacement tokens for restricted page related data.
 */

/**
 * Implements hook_token_info().
 */
function restrict_page_ip_token_info() {
  $info = array();
  // Define a new token type.
  $info['types']['restrict_page_ip'] = array(
    'name' => t('Restrict page Tokens'),
    'description' => t('A token type for Restrict Page by IP.'),
  );
  $info['tokens']['restrict_page_ip']['page'] = array(
    'name' => t('Restricted Page'),
    'description' => t('The Page restricted to access from specific IP addresses.'),
  );
  $info['tokens']['restrict_page_ip']['ip'] = array(
    'name' => t('Pathauto'),
    'description' => t('The IP restricted to access the page.'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function restrict_page_ip_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'restrict_page_ip') {
    // Loop through the available tokens.
    foreach ($tokens as $name => $token) {
      switch ($name) {
        case 'page':
          // Work out the value of our token.
          $value = $data['page'];
          // Give our token it's value.
          $replacements[$token] = "'" . $value . "'";
          break;

        case 'ip':
          // Work out the value of our token.
          $value = $data['ip'];
          // Give our token it's value.
          $replacements[$token] = $value;
          break;
      }
    }
  }

  return $replacements;
}
