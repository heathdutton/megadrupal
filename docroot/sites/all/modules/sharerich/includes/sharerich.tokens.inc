<?php
/**
 * @file
 * Tokens for Sharerich.
 */

/**
 * Implements hook_token_info().
 */
function sharerich_token_info() {
  $type = array(
    'name' => t('Sharerich Tokens'),
    'description' => t('Tokens used to create the share buttons.'),
  );

  $token['fb_app_id'] = array(
    'name' => t('Facebook App Id'),
    'description' => t("The App Id used to display the share box."),
  );
  $token['fb_site_url'] = array(
    'name' => t("Facebook site Url"),
    'description' => t("The Url used to display the share box."),
  );
  $token['youtube_username'] = array(
    'name' => t("Youtube Username"),
    'description' => t("Used to display the share box."),
  );
  $token['github_username'] = array(
    'name' => t("Github Username"),
    'description' => t("Used to display the share box."),
  );

  return array(
    'types' => array('sharerich' => $type),
    'tokens' => array('sharerich' => $token),
  );
}

/**
 * Implements hook_tokens().
 */
function sharerich_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'sharerich') {
    foreach ($tokens as $name => $original) {
      $value = '';
      switch ($name) {
        case 'fb_app_id':
          $value = check_plain(variable_get('sharerich_facebook_app_id'));
          break;

        case 'fb_site_url':
          $value = check_plain(variable_get('sharerich_facebook_site_url'));
          break;

        case 'youtube_username':
          $value = check_plain(variable_get('sharerich_youtube_username'));
          break;

        case 'github_username':
          $value = check_plain(variable_get('sharerich_github_username'));
          break;

        case 'url':
          if (isset($data['node']) && !empty($data['node'])) {
            $value = token_replace('[node:url]', $data, $options);
          }
          else {
            $value = token_replace('[current-page:url]');
          }
          $value = rawurlencode($value);
          break;

        case 'title':
          if (isset($data['node']) && !empty($data['node'])) {
            $value = token_replace('[node:title]', $data, $options);
          }
          else {
            $value = token_replace('[current-page:title]');
          }
          $value = rawurlencode(decode_entities($value));
          break;

        case 'summary':
          if (isset($data['node']) && !empty($data['node'])) {
            $value = token_replace('[node:summary]', $data, $options);
          }
          else {
            $value = token_replace('[site:name]');
          }
          $value = rawurlencode(decode_entities($value));
          break;
      }
      $replacements[$original] = $value;
    }

    return $replacements;
  }
}
