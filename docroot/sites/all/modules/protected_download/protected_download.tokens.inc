<?php

/**
 * @file
 * Provides token support for protected downloads.
 */

/**
 * Implements hook_token_info().
 */
function protected_download_token_info() {
  $file['protected-download-expire'] = array(
    'name' => t('Protected download expiry date'),
    'description' => t('The date when a protected download generated now will expire.'),
  );

  $file['protected-download-url'] = array(
    'name' => t('Protected download URL'),
    'description' => t('The web-accessible URL for the file protected with an authentication code.'),
  );

  return array(
    'types' => array(),
    'tokens' => array(
      'file' => $file,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function protected_download_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $language_code = isset($options['language']) ? $options['language']->language : NULL;
  $sanitize = !empty($options['sanitize']);

  $replacements = array();
  if ($type === 'file' && !empty($data['file'])) {
    $file = $data['file'];
    $scheme = file_uri_scheme($file->uri);
    $settings = protected_download_settings($scheme);
    $protected_download_expire = protected_download_expire_create_from_settings($settings);

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'protected-download-url':
          $url = protected_download_create_url($file->uri);
          $replacements[$original] = $sanitize ? check_plain($url) : $url;
          break;

        case 'protected-download-expire':
          $replacements[$original] = format_date($protected_download_expire, 'medium', '', NULL, $language_code);
          break;
      }
    }

    if ($date_tokens = token_find_with_prefix($tokens, 'protected-download-expire')) {
      $replacements += token_generate('date', $date_tokens, array('date' => $protected_download_expire), $options);
    }
  }

  return $replacements;
}
