<?php

/**
 * @file
 * Rules integration file for node accessibility.
 */

/**
 * Implements hook_rules_file_info().
 */
function node_accessibility_rules_file_info() {
  $items = array();
  $items[] = 'node_accessibility.rules';

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function node_accessibility_rules_condition_info() {
  $items = array();

  $items['content_is_using_node_accessibility'] = array(
    'group' => t("Node"),
    'label' => t("Content is using accessibility validation"),
    'base' => 'node_accessibility_rules_condition_content_is_using_node_accessibility',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t("Content")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

  $items['content_is_requiring_node_accessibility_validation'] = array(
    'group' => t("Node"),
    'label' => t("Content is requiring accessibility validation"),
    'base' => 'node_accessibility_rules_condition_content_is_requiring_node_accessibility_validation',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t("Content")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

  return $items;
}

/**
 * Implements hook_rules_event_info() on behalf of the node module.
 */
function node_accessibility_rules_event_info() {
  $items = array(
    'node_accessibility_after_validating' => array(
      'label' => t("After accessibility validating content"),
      'group' => t("Node"),
      'variables' => rules_events_node_variables(t("Accessibility validated content"), FALSE),
      'access callback' => 'rules_node_integration_access',
    ),
  );

  $items['node_accessibility_after_validating']['variables']['validation_results'] = array(
    'type' => 'unknown',
    'label' => t("Validation results"),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function node_accessibility_rules_action_info() {
  $items = array(
    'node_accessibility_validate_action' => array(
      'label' => t("Accessibility validate content"),
      'group' => t("Node"),
      'base' => 'node_accessibility_validate_rules_action',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t("Content that will be accessibility validated"),
        ),
      ),
      'provides' => array(
        'validation_results' => array(
          'type' => 'unknown',
          'label' => t("Validation results"),
        ),
      ),
    ),
   'node_accessibility_delete_action' => array(
      'label' => t("Delete accessibility statistics for content"),
      'group' => t("Node"),
      'base' => 'node_accessibility_delete_rules_action',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t("Content to delete statistics from"),
        ),
      ),
    ),
  );

  return $items;
}

/**
 * Action: Delete accessibility validation statistics for a node.
 *
 * $param object $node
 *   A node object.
 */
function node_accessibility_delete_rules_action($node) {
  if (!is_object($node)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('node');
    }

    return array('node' => $node);
  }

  actions_do('node_accessibility_delete_action', $node);

  return array('node' => $node);
}

/**
 * Action: Perform accessibility validation on a node.
 *
 * $param object $node
 *   A node object.
 *
 * @return
 *   An array containing the node object stored in the key called 'node' and the
 *   validation results (if any) stored in the key called 'results'.
 */
function node_accessibility_validate_rules_action($node) {
  if (!is_object($node)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('node');
    }

    return array('node' => $node);
  }

  $results = actions_do('node_accessibility_validate_action', $node);

  if (!isset($results['node_accessibility_validate_action'])) {
    return array('node' => $node, 'validation_results' => $results['node_accessibility_validate_action']);
  }

  return array('node' => $node);
}

/**
 * Condition: Check if the content is using accessibility validation.
 *
 * @param object $node
 *   A node object.
 *
 * @return bool
 *   TRUE/FALSE depending on if the content is using node accessibility
 *   validation.
 */
function node_accessibility_rules_condition_content_is_using_node_accessibility($node) {
  if (!is_object($node)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('node');
    }

    return FALSE;
  }

  return node_accessibility_is_enabled($node->type);
}

/**
 * Condition: Check if the content is requiring accessibility validation.
 *
 * @param object $node
 *   A node object.
 *
 * @return bool
 *   TRUE/FALSE depending on if the content is requiring node accessibility
 *   validation.
 */
function node_accessibility_rules_condition_content_is_requiring_node_accessibility_validation($node) {
  if (!is_object($node)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('node');
    }

    return FALSE;
  }

  return node_accessibility_is_required($node->type);
}
