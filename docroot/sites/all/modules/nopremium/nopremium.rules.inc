<?php
/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */



/**
 * Implementation of hook_rules_condition_info().
 */
function nopremium_rules_condition_info() {
  $defaults = array(
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'group' => t('Node'),
    'access callback' => 'rules_node_integration_access',
  );
  $items['node_is_premium'] = $defaults + array(
    'label' => t('Content is premium'),
    'base' => 'rules_condition_node_is_premium',
  );
  return $items;
}



/**
 * Condition: Check if the node is premium content.
 */
function rules_condition_node_is_premium($node) {
  return $node->premium == NODE_PREMIUM;
}



/**
 * Implementation of hook_rules_action_info().
 */
function nopremium_rules_action_info() {
  $defaults = array(
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content'), 'save' => TRUE),
    ),
    'group' => t('Node'),
    'access callback' => 'rules_node_admin_access',
  );
  $core_actions = nopremium_action_info();
  $actions = array('node_make_premium_action', 'node_make_non_premium_action');
  foreach ($actions as $base) {
    $action_name = str_replace('_action', '', $base);
    $items[$action_name] = $defaults + array(
      'label' => $core_actions[$base]['label'],
      'base' => $base,
    );
  }
  return $items;
}
