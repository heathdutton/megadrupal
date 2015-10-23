<?php

/**
 * @file
 * Token integration.
 */

/**
 * Implementation of hook_token_info().
 */
function entity_email_token_info() {
  $types['entity-email-from'] = array(
    'name' => t('Email From'),
    'description' => t('Tokens related to the user who send the email.'),
    'type' => 'user',
  );
  $types['entity-email-to'] = array(
    'name' => t('Email To'),
    'description' => t('Tokens related to the user who receive the email.'),
    'type' => 'user',
  );
  $types['entity-email-node'] = array(
    'name' => t('Node'),
    'description' => t('Tokens related to the node associated to this email.'),
    'type' => 'node',
  );
  $types['entity-email-dynamic'] = array(
    'name' => t('Dynamic'),
    'description' => t('Tokens related to the dynamic array.'),
    'type' => 'array',
  );
  return array(
    'types' => $types,
  );
}

/**
 * Implements hook_tokens().
 */
function entity_email_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'entity-email-dynamic' && isset($data['entity-email-dynamic'])) {
    $replacements += token_generate('array', $tokens, array('array' => $data['entity-email-dynamic']), $options);
  }

  if ($type == 'entity-email-from' && isset($data['entity-email-from'])) {
    $replacements += token_generate('user', $tokens, array('user' => $data['entity-email-from']), $options);
  }

  if ($type == 'entity-email-to' && isset($data['entity-email-to'])) {
    $replacements += token_generate('user', $tokens, array('user' => $data['entity-email-to']), $options);
  }

  if ($type == 'entity-email-node' && isset($data['entity-email-node'])) {
    $replacements += token_generate('node', $tokens, array('node' => $data['entity-email-node']), $options);
  }

  return $replacements;
}
