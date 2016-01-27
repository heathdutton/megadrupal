<?php

/**
 * @file
 * Rules integration with custom publishing options module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_condition_info()
 */
function custom_pub_rules_condition_info() {
  $items = array();
  $items['custom_pub_node_has_option'] = array(
    'label' => t('Content has custom publishing option'),
    'parameter' => array(
      'node' => array(
      'type' => 'node',
      'label' => t('Content'),
    ),
    'option' => array(
      'type' => 'text',
      'label' => t('Custom publishing option'),
      'options list' => 'custom_pub_types_list',
    ),
  ),
    'group' => t('Node'),
    'access callback' => 'rules_node_integration_access',
    'base' => 'custom_pub_rules_condition_node_has_option',
  );

  return $items;
}

function custom_pub_rules_condition_node_has_option($node, $option) {
  return (bool)$node->$option;
}