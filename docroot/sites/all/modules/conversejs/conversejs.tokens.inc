<?php

/**
 * @file
 * Token callbacks for the conversejs module.
 */

/**
 * Implements hook_token_info().
 *
 * @ingroup conversejs
 */
function conversejs_token_info() {
  $info = array();

  // Provides [user:password] token.
  $info['tokens']['user']['password'] = array(
    'name' => t('Encrypted password'),
    'description' => t('Encrypted user password as saved in database.'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 *
 * @ingroup conversejs
 */
function conversejs_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'user' && !empty($data['user'])) {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'password':
          $query = db_select('users', 'u')
            ->fields('u', array('pass'))
            ->condition('uid', $data['user']->uid)
            ->execute()
            ->fetchAll();
          $password = $query[0]->pass;

          if (isset($options['start']) or isset($options['length'])) {
            $start = isset($options['start']) ? $options['start'] : 0;
            if (isset($options['length'])) {
              $password = substr($password, $start, $options['length']);
            }
            else {
              $password = substr($password, $start);
            }
          }

          $replacements[$original] = $password;
          break;
      }
    }
  }

  return $replacements;
}
