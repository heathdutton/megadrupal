<?php
/**
 * @file
 * Builds user tokens for the recipient token type.
 */

/**
 * Implements hook_token_info().
 */
function manymail_token_info() {
  $info['types']['manymail-recipient'] = array(
    'name' => t('E-mail recipient'),
    'description' => t("Tokens related to the recipient's user account."),
    'type' => 'user',
    'needs-data' => 'manymail-recipient',
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function manymail_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'manymail-recipient' && !empty($data['manymail-recipient']) && $data['manymail-recipient'] instanceof ManyMailRecipient) {
    $recipient = $data['manymail-recipient'];

    if ($recipient->uid) {
      $recipient_user = user_load($recipient->uid);
      $replacements += token_generate('user', $tokens, array('user' => $recipient_user), $options);
    }
  }

  return $replacements;
}
