<?php

/**
 * Implements hook_token_info().
 */
function ecard_token_info() {

  $types['ecard'] = array(
    'name' => t('Ecard'),
    'description' => t('Tokens for the ecard module'),
  );

  $ecard['name_from'] = array(
    'name' => t("Sender's name"),
    'description' => t('The name of the ecard sender.'),
  );

  $ecard['mail_from'] = array(
    'name' => t("Sender's email address"),
    'description' => t('The email adress of the ecard sender.'),
  );

  $ecard['name_to'] = array(
    'name' => t("Recipient's name"),
    'description' => t('The name of the ecard recipient.'),
  );
  $ecard['mail_to'] = array(
    'name' => t("Recipient's email address"),
    'description' => t('The mail adress of the ecard recipient.'),
  );
  $ecard['ecard_url'] = array(
    'name' => t('Ecard URL'),
    'description' => t('The URL of the ecard.'),
  );
  $ecard['ecard_copy_url'] = array(
    'name' => t('Ecard copy URL'),
    'description' => t('The URL of the ecard copy.'),
  );

  return array(
    'types' => $types,
    'tokens' => array(
      'ecard' => $ecard,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function ecard_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type === 'ecard') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'name_from':
          $replacements[$original] = $data['ecard']->name_from;
          break;
        case 'mail_from':
          $replacements[$original] = $data['ecard']->mail_from;
          break;
        case 'name_to':
          $replacements[$original] = $data['ecard']->name_to;
          break;
        case 'mail_to':
          $replacements[$original] = $data['ecard']->mail_to;
          break;
        case 'ecard_url':
          $replacements[$original] = $data['ecard']->ecard_url;
          break;
        case 'ecard_copy_url':
          $replacements[$original] = $data['ecard']->ecard_copy_url;
          break;
      }
    }
  }
  return $replacements;
}
