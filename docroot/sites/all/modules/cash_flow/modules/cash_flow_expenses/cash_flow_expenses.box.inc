<?php
/**
 * @file
 * cash_flow_expenses.box.inc
 */

/**
 * Implements hook_default_box().
 */
function cash_flow_expenses_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'expense_node_shortcuts';
  $box->plugin_key = 'simple';
  $box->title = 'Shortcuts';
  $box->description = 'Shows links to do stuff related to expense content.';
  $box->options = array(
    'body' => array(
      'value' => '<a href="#block-views-payments-block-1">Jump to payments area</a>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['expense_node_shortcuts'] = $box;

  return $export;
}
