<?php

/**
 * @file
 * Rules integration for State Flow.
 */

/**
 * Implements hook_rules_event_info().
 *
 * Define Rules events that are triggered by State Flow.
 */
function state_flow_rules_event_info() {
  $items = array(
    'state_flow_event_fired' => array(
      'label' => t('After a workflow transition occurs'),
      'group' => t('State Flow'),
      'variables' => rules_events_node_variables(t('transitioned content')) + array(
        'author' => array('type' => 'user', 'label' => t('content author')),
        'workflow_state' => array('type' => 'text', 'label' => t('state')),
      ),
    ),
  );
  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function state_flow_rules_condition_info() {
  $items = array();
  $items['state_flow_state'] = array(
    'label' => t('Content is in state'),
    'group' => t('State Flow'),
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Content'),
        'description' => t('The node content for which the current state should be checked.'),
      ),
      'workflow_state' => array(
        'type' => 'text',
        'label' => t('State'),
      ),
    ),
    'base' => 'state_flow_rules_condition_node_is_state',
  );
  return $items;
}

/**
 * Callback for the "Content is in state" Rules condition.
 */
function state_flow_rules_condition_node_is_state($node, $state) {
  $state_flow = state_flow_load_state_machine($node);
  if (!$state_flow->ignore()) {
    return $state_flow->get_current_state() === $state;
  }
  return FALSE;
}
