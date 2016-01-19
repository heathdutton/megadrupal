<?php
/**
 * @file
 * Rules integration for the protected_node module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function protected_node_rules_rules_condition_info() {
  $items = array();
  $defaults = array(
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Content'),
      ),
    ),
    'group' => t('Protected Node'),
  );
  $items['protected_node_rules_condition_content_is_protected'] = array(
    'label' => t('Content is protected'),
    'help' => t('Evaluates to TRUE when the node is protected by a password.'),
  ) + $defaults;
  $items['protected_node_rules_condition_content_has_password'] = array(
    'label' => t('Content has password'),
    'help' => t('Evaluates to TRUE when the node has a password.'),
  ) + $defaults;
  $items['protected_node_rules_condition_content_is_locked'] = array(
    'label' => t('Content is locked from current user'),
    'help' => t('Evaluates to TRUE when the node is locked by a password from the current user.'),
  ) + $defaults;
  return $items;
}

/**
 * Condition: check whether the node is protected by password.
 */
function protected_node_rules_condition_content_is_protected($node, $settings) {
  return $node->protected_node_is_protected == 1;
}

/**
 * Condition: check whether the node has a password.
 */
function protected_node_rules_condition_content_has_password($node, $settings) {
  return !empty($node->protected_node_passwd);
}

/**
 * Condition: check whether the current user has access to the node.
 */
function protected_node_rules_condition_content_is_locked($node) {
  return protected_node_is_locked($node->nid) !== FALSE;
}

/**
 * Implements hook_rules_action_info().
 */
function protected_node_rules_rules_action_info() {
  $defaults = array(
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Content'),
      ),
    ),
    'group' => t('Protected node'),
  );
  $items['protected_node_rules_action_protect'] = array(
    'label' => t('Protect node'),
  ) + $defaults;
  $items['protected_node_rules_action_password_protect'] = array(
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Content'),
      ),
      'passwd' => array(
        'type' => 'text',
        'label' => t('Password'),
        'optional' => TRUE,
        'description' => t('Leave empty to set with a new random password.'),
      ),
      'show_title' => array(
        'type' => 'boolean',
        'label' => t('Show title'),
      ),
    ),
    'label' => t('Password protect node'),
  ) + $defaults;
  $items['protected_node_rules_action_unprotect'] = array(
    'label' => t('Unprotect the node'),
  ) + $defaults;
  $items['protected_node_rules_action_lock'] = array(
    'label' => t('Lock the node away from current user'),
  ) + $defaults;
  $items['protected_node_rules_action_unlock'] = array(
    'label' => t('Unlock the node for current user'),
  ) + $defaults;
  return $items;
}

/**
 * Activate the protection on the specified node.
 */
function protected_node_rules_action_protect($node) {
  $node->protected_node_is_protected = 1;
  return array('node' => $node);
}

/**
 * Activate the password protection on the specified node.
 */
function protected_node_rules_action_password_protect($node, $passwd, $show_title) {
  $node->protected_node_is_protected = 1;
  $node->protected_node_show_title = $show_title;
  // Note: the following test prevents the user from using "0" as a password.
  $node->protected_node_passwd = $passwd ? $passwd : user_password();
  return array('node' => $node);
}

/**
 * Deactivate the protection on the specified node.
 */
function protected_node_rules_action_unprotect($node) {
  $node->protected_node_is_protected = 0;
  return array('node' => $node);
}

/**
 * Revoke any access right to the node from the current user.
 */
function protected_node_rules_action_lock($node) {
  protected_node_lock($node->nid);
  return array();
}

/**
 * Grant access rights to the node to the current user.
 */
function protected_node_rules_action_unlock($node) {
  protected_node_unlock($node->nid);
  return array();
}
