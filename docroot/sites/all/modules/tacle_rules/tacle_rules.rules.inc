<?php

/**
 * @file
 * Rules integration with the Taxonomy Access Lite (TACL) module.
 */
define('TACLE_RULES_TAC_LITE_NAME', 'Taxonomy Access Control Lite');

/**
 * Implements hook_rules_action_info().
 */
function tacle_rules_rules_action_info() {
  return array(
    'tacle_rules_action_single_term_role_permission' => array(
      'label' => t('Modify single term access for role(s)'),
      'group' => TACLE_RULES_TAC_LITE_NAME,
      'access callback' => 'tacle_rules_access',
      'parameter' => array(
        'permission' => array(
          'type' => 'text',
          'label' => t('Permsission'),
          'default value' => 'grant',
          'options list' => '_tacle_rules_options_permission',
        ),
        'roles' => array(
          'type' => 'list<integer>',
          'label' => t('Roles'),
          'description' => t('Select the roles to modify permission for. Hold [Ctrl] to select multiple roles.'),
          'options list' => 'entity_metadata_user_roles',
        ),
        'schemes' => array(
          'type' => 'list<integer>',
          'label' => t('Schemes'),
          'description' => t('Select the schemes to modify permission for. Hold [Ctrl] to select multiple schemes.'),
          'options list' => '_tacle_rules_options_schemes',
        ),
        'taxonomy_term' => array(
          'type' => 'taxonomy_term',
          'label' => t('Taxonomy term'),
        ),
      ),
    ),
    'tacle_rules_action_multiple_term_role_permission' => array(
      'label' => t('Modify multiple term access for role(s)'),
      'group' => TACLE_RULES_TAC_LITE_NAME,
      'access callback' => 'tacle_rules_access',
      'parameter' => array(
        'permission' => array(
          'type' => 'text',
          'label' => t('Permsission'),
          'default value' => 'grant',
          'options list' => '_tacle_rules_options_permission',
        ),
        'roles' => array(
          'type' => 'list<integer>',
          'label' => t('Roles'),
          'description' => t('Select the roles to modify permission for. Hold [Ctrl] to select multiple roles.'),
          'options list' => 'entity_metadata_user_roles',
        ),
        'schemes' => array(
          'type' => 'list<integer>',
          'label' => t('Schemes'),
          'description' => t('Select the schemes to modify permission for. Hold [Ctrl] to select multiple schemes.'),
          'options list' => '_tacle_rules_options_schemes',
        ),
        'taxonomy_terms' => array(
          'type' => 'list<taxonomy_term>',
          'label' => t('Taxonomy terms'),
        ),
      ),
    ),
    'tacle_rules_action_single_term_user_permission' => array(
      'label' => t('Modify single term access for user'),
      'group' => TACLE_RULES_TAC_LITE_NAME,
      'access callback' => 'tacle_rules_access',
      'parameter' => array(
        'permission' => array(
          'type' => 'text',
          'label' => t('Permsission'),
          'default value' => 'grant',
          'options list' => '_tacle_rules_options_permission',
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
        'schemes' => array(
          'type' => 'list<integer>',
          'label' => t('Schemes'),
          'description' => t('Select the schemes to modify permission for. Hold [Ctrl] to select multiple schemes.'),
          'options list' => '_tacle_rules_options_schemes',
        ),
        'taxonomy_term' => array(
          'type' => 'taxonomy_term',
          'label' => t('Taxonomy term'),
        ),
      ),
    ),
    'tacle_rules_action_multiple_term_user_permission' => array(
      'label' => t('Modify multiple term access for user'),
      'group' => TACLE_RULES_TAC_LITE_NAME,
      'access callback' => 'tacle_rules_access',
      'parameter' => array(
        'permission' => array(
          'type' => 'text',
          'label' => t('Permsission'),
          'default value' => 'grant',
          'options list' => '_tacle_rules_options_permission',
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
        'schemes' => array(
          'type' => 'list<integer>',
          'label' => t('Schemes'),
          'description' => t('Select the schemes to modify permission for. Hold [Ctrl] to select multiple schemes.'),
          'options list' => '_tacle_rules_options_schemes',
        ),
        'taxonomy_terms' => array(
          'type' => 'list<taxonomy_term>',
          'label' => t('Taxonomy terms'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function tacle_rules_rules_condition_info() {
  return array(
    'tacle_rules_condition_is_tacl_term' => array(
      'label' => t('Check term is controlled by TACL'),
      'group' => TACLE_RULES_TAC_LITE_NAME,
      'parameter' => array(
        'taxonomy_term' => array(
          'type' => 'taxonomy_term',
          'label' => t('Taxonomy term'),
        ),
      ),
    ),
    'tacle_rules_condition_is_tacl_vocabulary' => array(
      'label' => t('Check vocabulary is controlled by TACL'),
      'group' => TACLE_RULES_TAC_LITE_NAME,
      'parameter' => array(
        'taxonomy_vocabulary' => array(
          'type' => 'taxonomy_vocabulary',
          'label' => t('Taxonomy vocabulary'),
        ),
      ),
    ),
    'tacle_rules_condition_user_has_tacl_access' => array(
      'label' => t('Check user account access rights to term(s)'),
      'group' => TACLE_RULES_TAC_LITE_NAME,
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User account'),
        ),
        'ops' => array(
          'type' => 'list<text>',
          'label' => t('Access operations'),
          'options list' => '_tacle_rules_options_operation',
        ),
        'taxonomy_terms' => array(
          'type' => 'list<taxonomy_term>',
          'label' => t('Taxonomy terms'),
        ),
      ),
    ),
  );
}

/**
 * Action: Modify single term access for role(s).
 *
 * @param string $permission
 * @param array $roles
 * @param array $schemes
 * @param object $taxonomy_term
 * @param array $settings
 */
function tacle_rules_action_single_term_role_permission($permission, $roles, $schemes, $taxonomy_term, $settings) {
  $taxonomy_terms = array($taxonomy_term->tid => $taxonomy_term);
  tacle_rules_action_multiple_term_role_permission($permission, $roles, $schemes, $taxonomy_terms, $settings);
}

/**
 * Action: Modify multiple term access for role(s).
 *
 * @param string $permission
 * @param array $roles
 * @param array $schemes
 * @param object $taxonomy_terms
 * @param array $settings
 */
function tacle_rules_action_multiple_term_role_permission($permission, $roles, $schemes, $taxonomy_terms, $settings) {
  $schemes = _tacle_rules_valid_schemes($schemes);
  $terms = _tacle_rules_valid_terms($taxonomy_terms);
  $permission = _tacle_rules_valid_permission($permission);
  $roles = array_filter($roles);

  $vids = array();
  foreach ($schemes as $scheme_id) {
    $access = variable_get('tac_lite_grants_scheme_' . $scheme_id, array());
    foreach ($roles as $role) {
      switch ($permission) {
        case 'grant':
          foreach ($terms as $term) {
            if (!isset($access[$role][$term->vid][$term->tid])) {
              $access[$role][$term->vid][$term->tid] = $term->tid;
              $vids[$term->vid] = $term->vid;
            }
          }
          break;

        case 'revoke':
          foreach ($terms as $term) {
            if (isset($access[$role][$term->vid][$term->tid])) {
              unset($access[$role][$term->vid][$term->tid]);
              $vids[$term->vid] = $term->vid;
            }
          }
          break;
      }
    }

    if (count($vids)) {
      foreach ($roles as $role) {
        // Changes have been made. TAC uses a placeholder when no access has been
        // set. Scan through each vocabulary to see if we should add or remove the
        // placeholder. Finally save the role access.
        foreach ($vids as $vid) {
          if (empty($access[$role][$vid])) {
            $access[$role][$vid][0] = 0;
          }
          else {
            unset($access[$role][$vid][0]);
          }
        }
      }
      variable_set('tac_lite_grants_scheme_' . $scheme_id, $access);
    }
  }
}

/**
 * Action: Modify single term access for a user.
 *
 * @param string $permission
 * @param object $account
 * @param array $schemes
 * @param object $taxonomy_term
 * @param array $settings
 */
function tacle_rules_action_single_term_user_permission($permission, $account, $schemes, $taxonomy_term, $settings) {
  $taxonomy_terms = array($taxonomy_term->tid => $taxonomy_term);
  tacle_rules_action_multiple_term_user_permission($permission, $account, $schemes, $taxonomy_terms, $settings);
}

/**
 * Action: Modify multiple term access for a user.
 *
 * @param string $permission
 * @param object $account
 * @param array $schemes
 * @param object $taxonomy_term
 * @param array $settings
 */
function tacle_rules_action_multiple_term_user_permission($permission, $account, $schemes, $taxonomy_terms, $settings) {
  $schemes = _tacle_rules_valid_schemes($schemes);
  $schemes_all = _tacle_rules_options_schemes();
  $terms = _tacle_rules_valid_terms($taxonomy_terms);
  $permission = _tacle_rules_valid_permission($permission);
  $access = array();
  $save = FALSE;

  if (count($schemes_all)) {
    foreach ($schemes_all as $id => $name) {
      $config = _tac_lite_config($id);
      $realm = $config['realm'];
      if (isset($account->data[$realm])) {
        $access[$realm] = $account->data[$realm];
      }
      else {
        $access[$realm] = array();
      }
      if (isset($schemes[$id])) {
        $schemes[$id] = $realm;
      }
    }
  }

  foreach ($schemes as $id => $realm) {
    $vids = array();
    switch ($permission) {
      case 'grant':
        foreach ($terms as $term) {
          if (!isset($access[$realm][$term->vid][$term->tid])) {
            if (!isset($access[$realm][$term->vid])) {
              $access[$realm][$term->vid] = array();
            }
            $access[$realm][$term->vid][$term->tid] = $term->tid;
            $vids[$term->vid] = $term->vid;
          }
        }
        break;

      case 'revoke':
        foreach ($terms as $term) {
          if (isset($access[$realm][$term->vid][$term->tid])) {
            unset($access[$realm][$term->vid][$term->tid]);
            $vids[$term->vid] = $term->vid;
          }
        }
        break;
    }

    if (count($vids)) {
      $save = TRUE;
      // Changes have been made. TAC uses a placeholder when no access has been
      // set. Scan through each vocabulary to see if we should add or remove the
      // placeholder. Finally save the access rights to the account.
      foreach ($vids as $vid) {
        if (empty($access[$realm][$vid])) {
          $access[$realm][$vid][0] = 0;
        }
        else {
          unset($access[$realm][$vid][0]);
        }
      }
    }
  }
  if ($save) {
    user_save($account, $access, 'tac_lite');
  }
}

/**
 * Condition: Check if vocabulary is controlled by TAC.
 *
 * @param object $taxonomy_vocabulary
 * @param array $settings
 * @return bool
 */
function tacle_rules_condition_is_tacl_vocabulary($taxonomy_vocabulary, $settings) {
  $vids = variable_get('tac_lite_categories', array());
  return in_array($taxonomy_vocabulary->vid, $vids);
}

/**
 * Condition: Check if term is controlled by TAC.
 *
 * @param object $taxonomy_term
 * @return bool
 */
function tacle_rules_condition_is_tacl_term($taxonomy_term, $settings) {
  $vids = variable_get('tac_lite_categories', array());
  return in_array($taxonomy_term->vid, $vids);
}

/**
 * Condition: Check if user account has access to TACL term.
 *
 * @param object $account
 * @param array $ops
 * @param array $taxonomy_terms
 * @param array $settings
 * @return bool
 */
function tacle_rules_condition_user_has_tacl_access($account, $ops, $taxonomy_terms, $settings) {
  $terms = $taxonomy_terms;
  $ops = array_filter($ops);
  $schemes = _tacle_rules_options_schemes();
  $schemes_loaded = array();
  foreach ($schemes as $id => $name) {
    $schemes_loaded[$id] = _tac_lite_config($id);
  }

  $access = TRUE;
  foreach ($ops as $op) {
    $grants = tac_lite_node_grants($account, $op);
    foreach ($schemes_loaded as $scheme) {
      foreach ($terms as $term) {
        if (!empty($scheme['perms']['grant_' . $op])) {
          if (empty($grants[$scheme['realm']][$term->tid])) {
            $access = FALSE;
          }
        }
      }
    }
  }
  return $access;
}

/**
 * Checks schemes array to make sure each scheme is still valid.
 *
 * @param array $schemes
 * @return
 *  Boolean FALSE on failure otherwise valid array schemes variable.
 */
function _tacle_rules_valid_schemes($schemes) {
  $schemes_all = _tacle_rules_options_schemes();
  if (empty($schemes_all)) {
    return FALSE;
  }

  $schemes_act = array_filter($schemes);
  if (empty($schemes_act)) {
    rules_log(t('%tac_lite scheme missing.', array('%tac_lite' => TACLE_RULES_TAC_LITE_NAME)));
    return FALSE;
  }

  $schemes_valid = array();
  foreach ($schemes_act as $id => $enabled) {
    if (!isset($schemes_all[$id])) {
      rules_log(t('%tac_lite scheme no longer exists.', array('%tac_lite' => TACLE_RULES_TAC_LITE_NAME)));
    }
    else {
      $schemes_valid[$id] = $enabled;
    }
  }
  return $schemes_valid;
}

/**
 * Checks terms to make sure they are valid.
 *
 * @param array $terms
 * @return
 *  Boolean FALSE on failure otherwise an array of valid terms.
 */
function _tacle_rules_valid_terms($terms) {
  $vids = _tacle_rules_options_vocabularies();
  if (empty($vids)) {
    return FALSE;
  }
  if (empty($terms)) {
    rules_log(t('TACLe Rules: No terms selected.'));
    return FALSE;
  }
  else {
    $terms_valid = array();
    foreach ($terms as $term) {
      if (empty($term->tid)) {
        rules_log(t('TACLe Rules: Term ID missing.'));
      }
      elseif (!in_array($term->vid, array_keys($vids))) {
        rules_log(t('TACLe Rules: Term ID !term_id not in %tac_lite controlled vocabulary.', array('%tac_lite' => TACLE_RULES_TAC_LITE_NAME, '!term_id' => $term->vid)));
      }
      else {
        $terms_valid[] = $term;
      }
    }
    return $terms_valid;
  }
  return FALSE;
}

/**
 * Checks permission variable to make sure it is valid.
 *
 * @param string $permission
 * @return
 *  Boolean FALSE on failure otherwise valid string permission variable.
 */
function _tacle_rules_valid_permission($permission) {
  if (empty($permission)) {
    rules_log(t('TACLe Rules: Permission action missing'));
    return FALSE;
  }
  if (!in_array($permission, array('grant', 'revoke'))) {
    rules_log(t('TACLe Rules: Permission action not grant or revoke'));
    return FALSE;
  }
  return $permission;
}

/**
 * Options list callback for permissions
 *
 * @return array
 */
function _tacle_rules_options_permission() {
  return array(
    'grant' => t('Grant'),
    'revoke' => t('Revoke'),
  );
}

/**
 * Options list callback for operations
 *
 * @return array
 */
function _tacle_rules_options_operation() {
  return array(
    'view' => t('View'),
    'update' => t('Update'),
    'delete' => t('Delete'),
  );
}

/**
 * Access callback for TACLe rules actions
 *
 * @return bool
 */
function tacle_rules_access() {
  return user_access('administer users');
}

// @todo: The functions would be better in the tac_lite module.

/**
 * Generate a list of vocabularies controlled by TACL. Return array keyed by
 * vocubulary ID with vocabulary name as value.
 *
 * @param bool $display_error
 * @return array
 */
function _tacle_rules_options_vocabularies($display_error = FALSE) {
  $vocabularies = taxonomy_get_vocabularies();
  $vids = variable_get('tac_lite_categories', array());
  $options = array();
  foreach ($vids as $vid) {
    $options[$vid] = $vocabularies[$vid]->name;
  }
  if (empty($options)) {
    $message = t('TACLe Rules: %tac_lite not configured to use any taxonomy vocabularies.', array('%tac_lite' => TACLE_RULES_TAC_LITE_NAME));
    rules_log($message);
    if ($display_error) {
      drupal_set_message($message, 'error');
    }
  }
  return $options;
}

/**
 * Generate a list of TACL schemes keyed by scheme ID with scheme name as value.
 *
 * @param bool $display_error
 * @return array
 */
function _tacle_rules_options_schemes($display_error = FALSE) {
  $schemes = array();
  $schemes_max = variable_get('tac_lite_schemes', 1);
  for ($i = 1; $i <= $schemes_max; ++$i) {
    $config = _tac_lite_config($i);
    // A scheme without a name is not enabled.
    if (!empty($config['name'])) {
      $schemes[$i] = $config['name'];
    }
  }
  if (empty($schemes)) {
    $message = t('TACLe Rules: %tac_lite not configured to use any schemes.', array('%tac_lite' => TACLE_RULES_TAC_LITE_NAME));
    rules_log($message);
    if ($display_error) {
      drupal_set_message($message, 'error');
    }
  }
  return $schemes;
}
