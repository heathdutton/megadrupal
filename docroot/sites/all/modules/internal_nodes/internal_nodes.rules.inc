 <?php
/**
 * @file
 * Internal nodes Rules Integration
 *
 * Actions/conditions
 * 1. Change node view setting - TODO
 *
 * Events
 * 1. Node view denied to user
 */

/**
 * Implements hook_rules_action_info().
 */
function internal_nodes_rules_action_info() {
}

/**
 * Implements hook_rules_event_info().
 */
function internal_nodes_rules_event_info() {
  $defaults = array(
    'group' => t('Internal nodes'),
    'module' => 'internal_nodes',
    'access callback' => 'internal_nodes_integration_access',
  );
  return array(
    'internal_nodes_denied' => $defaults + array(
      'label' => t('Node view denied to user'),
      'variables' => array(
        'node' => array('type' => 'node', 'label' => t('denied content')),
      ),
    ),
  );
}

/**
 * Internal nodes integration access callback.
 */
function internal_nodes_integration_access($type, $name) {
  if ($type == 'event') {
    return entity_access('view', 'node');
  }
}
