<?php
/**
 * @file
 * Catches tokens for the recipient token type that have not been replaced
 * yet (due to lack of UID) and replaces them with custom entered data.
 */

/**
 * Implements hook_tokens().
 */
function manymail_lists_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'manymail-recipient' && !empty($data['manymail-recipient']) && $data['manymail-recipient'] instanceof ManyMailRecipient) {
    $recipient = $data['manymail-recipient'];

    if (!$recipient->uid) {
      $sanitize = !empty($options['sanitize']);

      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'mail':
            $replacements[$original] = $sanitize ? filter_xss($recipient->mail) : $recipient->mail;
            break;

          case 'name':
            $replacements[$original] = $sanitize ? filter_xss($recipient->name) : $recipient->name;
            break;

        }
      }
    }
  }

  return $replacements;
}
