<?php

/**
 * @file
 * Integration of Private Taxonomy with Tokens.
 */

/**
 * Implements hook_token_info().
 */
function private_taxonomy_token_info() {
  $types['term'] = array();

  $private_taxonomy['term_owner_name'] = array(
    'name' => t("Private Term Owner Name"),
    'description' => t("The {user}.name of the term owner."),
  );
  $private_taxonomy['term_owner_uid'] = array(
    'name' => t("Private Term Owner UID"),
    'description' => t("The {user}.uid of the term owner."),
  );

  return array(
    'types' => $types,
    'tokens' => array(
      'term' => $private_taxonomy,
    ),
  );
}


/**
 * Implements hook_tokens().
 */
function private_taxonomy_tokens($type, $tokens, array $data = array(), array $options = array()) {

  $replacements = array();
  if ($type == 'term') {
    foreach ($tokens as $name => $original) {
      if (isset($data['term'])) {
        switch ($name) {
          case 'term_owner_name':
            $tid = $data['term']->tid;
            $sql = 'SELECT uid FROM {user_term} WHERE tid = :tid';
            $uid = intval(db_query($sql, array(':tid' => $tid))->fetchField());
            $user = user_load($uid);
            $name = $user->name;
            $replacements[$original] = $name;
            break;

          case 'term_owner_uid':
            $tid = $data['term']->tid;
            $sql = 'SELECT uid FROM {user_term} WHERE tid = :tid';
            $uid = intval(db_query($sql, array(':tid' => $tid))->fetchField());
            $replacements[$original] = $uid;
            break;
        }
      }
    }
  }

  return $replacements;
}
