<?php
/**
 * @file
 * Contains rules events, conditions and actions for the Giftwrap Module.
 */

/*
 * Implements hook_rules_event_info().
 */
 
function commerce_giftwrap_rules_event_info() {
  // The line item rule file is needed for the access callback.
  module_load_include('inc', 'commerce_line_item', 'commerce_line_item.rules');
  $items = array(
    'commerce_giftwrap_calculate' => array(
      'label' => t('Calculate the giftwrap price for a line item'),
      'group' => t('Commerce Giftwrap'),
      'variables' => entity_rules_events_variables('commerce_line_item', t('Line Item', array(), array('context' => 'a drupal commerce line item'))),
      'access callback' => 'commerce_line_item_rules_access',
    ),
  );
  return $items;
}