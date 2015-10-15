<?php
/**
 * Implements hook_rules_condition_info() on behalf of the user module.
 */
function uuidrole_rules_condition_info() {
  return array(
    'user_has_roleuuid' => array(
      'label' => t('User has role(s) UUID'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'roles' => array(
          'type' => 'list<text>',
          'label' => t('Roles'),
          'options list' => 'uuidrole_roles_options',
        ),
        'operation' => array(
          'type' => 'text',
          'label' => t('Match roles'),
          'options list' => 'rules_user_condition_operations',
          'restriction' => 'input',
          'optional' => TRUE,
          'default value' => 'AND',
          'description' => t('If matching against all selected roles, the user must have <em>all</em> the roles selected.'),
        ),
      ),
      'group' => t('User'),
      'access callback' => 'rules_user_integration_access',
      'base' => 'uuidroles_rules_condition_user_has_roleuuid',
    ),
  );
}

/**
 * Options list callback for user roles uuid.
 */
function uuidrole_roles_options() {
  return uuidrole_roles();
}

/**
 * Condition user: condition to check whether user has particular roles
 */
function uuidroles_rules_condition_user_has_roleuuid($account, $roles, $operation = 'AND') {
  switch ($operation) {
    case 'OR':
      foreach ($roles as $uuid) {
        if (isset($account->uuidroles[$uuid])) {
          return TRUE;
        }
      }
      return FALSE;

    case 'AND':
      foreach ($roles as $uuid) {
        if (!isset($account->uuidroles[$uuid])) {
          return FALSE;
        }
      }
      return TRUE;
  }
}
