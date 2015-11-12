<?php

/**
 * @file
 * Builds placeholder replacement tokens for invites-related data.
 */

/**
 * Implements hook_token_info().
 */
function invites_token_info() {
  $types['invites'] = array(
    'name' => t('Invites'),
    'description' => t('Tokens related to individual invites accounts.'),
    'needs-data' => 'invites',
  );

  $invites['mail-url'] = array(
    'name' => t("Mail URL"),
    'description' => t("The URL of the invitation mail."),
  );
  $invites['user-url'] = array(
    'name' => t("User Page URL"),
    'description' => t("The URL of the user page."),
  );
  $invites['inviter-name'] = array(
    'name' => t("Inviter Name"),
    'description' => t("The inviter of the invitation."),
  );
  $invites['inviter-mail'] = array(
    'name' => t("Inviter Mail"),
    'description' => t("The inviter email."),
  );

  return array(
    'types' => $types,
    'tokens' => array('invites' => $invites),
  );
}

/**
 * Implements hook_tokens().
 */
function invites_tokens($type, $tokens, array $data = array(), array $options = array()) {
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

  if ($type == 'invites' && !empty($data['invites'])) {
    $invites = $data['invites'];
    $invites_token = $invites->invites_token;
    $inviter = user_load($invites->inviter);
    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic invites account information.      
        case 'mail-url':
          $replacements[$original] = url("invitation/{$invites_token}", $url_options);
          break;

        case 'user-url':
          $replacements[$original] = url("user/", $url_options);
          break;

        case 'inviter-name':
          $replacements[$original] = $inviter->name;
          break;

        case 'inviter-mail':
          $replacements[$original] = $inviter->mail;
          break;
      }
    }
  }
  return $replacements;
}
