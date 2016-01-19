<?php

/**
 * @file
 * Rules integration for the Rules nodejs action module.
 */

/**
 * Implements hook_rules_action_info().
 */
function rules_nodejs_action_rules_action_info() {
  $items = array();
  $items['rules_nodejs'] = array(
    'label' => t('Action node.js'),
    'group' => t('node.js'),
    'parameter' => array(
      'subject' => array(
        'type' => 'text',
        'label' => t('The subject of action message.'),
        'description' => t('One can use tokens or simple text.'),
        'default value' => FALSE,
      ),
      'body' => array(
        'type' => 'text',
        'label' => t('The body of action message.'),
        'description' => t('One can use tokens or simple text.'),
        'default value' => FALSE,
      ),
      'picture' => array(
        'type' => 'text',
        'label' => t('User picture'),
        'description' => t('Put user uid or token(which will be converted to uid) whose picture will be shown in action message. Example: 2 or [account:uid] or "current".'),
        'default value' => FALSE,
        'optional' => TRUE,
      ),
      'roles' => array(
        'type' => 'list<integer>',
        'label' => t('Roles'),
        'description' => t('One can select roles to which action message is going too.'),
        'options list' => 'rules_nodejs_action_roles_list',
        'optional' => TRUE,
      ),
      'users' => array(
        'type' => 'text',
        'label' => t('Users uids'),
        'description' => t('Put uids of users to which action message is going to be sent. Comma-separated values. Example: 2, 3, [account:uid], 35.'),
        'optional' => TRUE,
      ),
    ),
    'base' => 'rules_nodejs_action_rules_process',
  );

  return $items;
}

/**
 * Action: Process and send Message.
 */
function rules_nodejs_action_rules_process($subject, $body, $picture, $roles = array(), $users = '') {
  $action = new RulesNodejsAction($subject, $body, $picture, $roles, $users);
  $action->send();
}

/**
 * Generate list of roles.
 */
function rules_nodejs_action_roles_list() {
  return array(DRUPAL_AUTHENTICATED_RID => t('All authenticated users')) + entity_metadata_user_roles();
}
