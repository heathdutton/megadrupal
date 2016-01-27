<?php

/**
 * @file
 * Token integration for the bookmark module.
 */

/**
 * Implements hook_token_info().
 */
function bookmark_token_info() {
  $info['types']['bookmark'] = array(
    'name' => t('Bookmark'),
    'description' => t('Tokens related to individual bookmarks.'),
    'needs-data' => 'bookmark',
  );

  $info['tokens']['bookmark']['bid'] = array(
    'name' => t('Bookmark ID'),
    'description' => t('The unique ID of the bookmark.'),
  );
  $info['tokens']['bookmark']['title'] = array(
    'name' => t('Title'),
    'description' => t('The title of the bookmark.'),
  );
  $info['tokens']['bookmark']['language'] = array(
    'name' => t('Language'),
    'description' => t('The language of the bookmark.'),
  );
  $info['tokens']['bookmark']['url'] = array(
    'name' => t('URL'),
    'description' => t('The URL of the bookmark.'),
  );
  //$info['tokens']['bookmark']['url-hash'] = array(
  //  'name' => t('URL'),
  //  'description' => t("The hash of the bookmark's URL."),
  //);
  $info['tokens']['bookmark']['edit-url'] = array(
    'name' => t('Edit URL'),
    'description' => t("The URL of the bookmark's edit page."),
    'type' => 'url',
  );
  $info['tokens']['bookmark']['user'] = array(
    'name' => t('User'),
    'description' => t('The user account that owns the bookmark.'),
    'type' => 'user',
  );
  $info['tokens']['bookmark']['created'] = array(
    'name' => t('Date created'),
    'description' => t('The date the bookmark was created.'),
    'type' => 'date',
  );
  $info['tokens']['bookmark']['changed'] = array(
    'name' => t('Date changed'),
    'description' => t('The date the bookmark was most recently updated.'),
    'type' => 'date',
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function bookmark_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'bookmark' && !empty($data['bookmark'])) {
    $bookmark = $data['bookmark'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'bid':
          $replacements[$original] = $bookmark->bid;
          break;

        case 'title':
          $replacements[$original] = $sanitize ? check_plain($bookmark->title) : $bookmark->title;
          break;

        case 'language':
          $replacements[$original] = $sanitize ? check_plain($bookmark->language) : $bookmark->language;
          break;

        case 'url':
          $replacements[$original] = url($bookmark->url);
          break;

        case 'url-brief':
          $brief = bookmark_url_get_brief($bookmark->url);
          $replacements[$original] = $sanitize ? check_plain($brief) : $brief;
          break;
        //case 'url-hash':
        //  $replacements[$original] = $bookmark->url_hash;
        //  break;

        case 'edit-url':
          $replacements[$original] = url("bookmark/{$bookmark->bid}/edit", $url_options);
          break;

        case 'user':
          $account = user_load($bookmark->uid);
          $name = format_username($account);
          $replacements[$original] = $sanitize ? check_plain($name) : $name;
          break;

        case 'created':
          $replacements[$original] = format_date($bookmark->created, 'medium', '', NULL, $language_code);
          break;

        case 'changed':
          $replacements[$original] = format_date($bookmark->changed, 'medium', '', NULL, $language_code);
          break;
      }
    }

    // Chained tokens.
    if ($user_tokens = token_find_with_prefix($tokens, 'user')) {
      $account = user_load($bookmark->uid);
      $replacements += token_generate('user', $user_tokens, array('user' => $account), $options);
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $created_tokens, array('date' => $bookmark->created), $options);
    }

    if ($changed_tokens = token_find_with_prefix($tokens, 'changed')) {
      $replacements += token_generate('date', $changed_tokens, array('date' => $bookmark->changed), $options);
    }
  }

  return $replacements;
}
