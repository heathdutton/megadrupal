<?php

/**
 * Export Drupal Commerce orders to Views.
 */

/**
 * Implements hook_views_data()
 */
function commerce_recurring_ui_views_data() {
  $data = array();

  $data['commerce_recurring']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the order.'),
      'handler' => 'commerce_recurring_handler_field_order_operations',
    ),
  );

  // Show the operations using CTools dropbutton menu.
  $data['commerce_recurring']['operations_dropbutton'] = array(
    'title' => t('Operations'),
    'help' => t('Show the commerce recurring operation links using dropbutton menu.'),
    'field' => array(
      'handler' => 'commerce_recurring_ui_handler_field_operations_dropbutton',
    ),
    // This is a dummy field, so attach it to the ID column.
    'real field' => 'id',
  );

  return $data;
}
