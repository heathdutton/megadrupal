<?php

/**
 * @file
 * Builds placeholder replacement tokens for users.
 */

/**
 * Implements hook_token_info().
 */
function uuid_user_revision_token_info() {
  $tokens = array();
  $tokens['revision-uuid'] = array(
    'name' => t('User Revision UUID'),
    'description' => t('The universally unique ID of the user revision.'),
  );

  return array(
    'tokens' => array('user' => $tokens),
  );
}

/**
 * Implements hook_tokens().
 */
function uuid_user_revision_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ('user' != $type) {
    return $replacements;
  }

  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'revision-uuid':
        $replacements[$original] = $data['user']->vuuid;
        break;
    }
  }
  return $replacements;
}
