<?php
/**
 * @file
 * Custom tokens for Google Authorship.
 */

/**
 * Implements hook_token_info().
 */
function google_authorship_token_info() {
  $info = array();

  $info['tokens']['node']['google_authorship_id'] = array(
    'name' => t('Google Authorship ID'),
    'description' => t("The Google+ ID of the node's author."),
  );
  $info['tokens']['node']['google_authorship_url'] = array(
    'name' => t('Google Authorship URL'),
    'description' => t("The full URL to the node author's Google+ profile page."),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function google_authorship_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  $sanitize = !empty($options['sanitize']);

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];
    foreach ($tokens as $name => $original) {
      if ($original == '[node:google_authorship_id]' || $original == '[node:google_authorship_url]') {
        $account = user_load($node->uid);
        $fields = field_get_items('user', $account, 'field_google_plus_id');
        if (!empty($fields)) {
          $field = reset($fields);
          if (!empty($field['safe_value']) && is_numeric($field['safe_value'])) {
            // The ID will be needed for either token.
            $replacements[$original] = $field['safe_value'];

            // If the URL is requested, put it together.
            if ($original == '[node:google_authorship_url]') {
              $replacements[$original] = GOOGLE_AUTHORSHIP_PREFIX . $replacements[$original];
            }
          }
        }
      }
    }
  }

  return $replacements;
}
