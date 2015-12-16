<?php

/**
 * @file
 * Defines Unix Time Conversion blocks.
 */

/**
 * Implements hook_block_info().
 */
function unix_time_conversion_block_info() {
  // Empty blocks array definition.
  $blocks = array();
  // Timestamp to date block.
  $blocks['unix_timestamp_to_date'] = array(
    'info' => t('Unix Timestamp To Date'),
  );
  // Date to timestamp block.
  $blocks['date_to_unix_timestamp'] = array(
    'info' => t('Date To Unix Timestamp'),
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function unix_time_conversion_block_view($delta = '') {
  // Empty block array definition.
  $block = array();
  switch ($delta) {
    // Timestamp to date block.
    case 'unix_timestamp_to_date':
      $block['subject'] = '';
      // Get form structure & render the form.
      $form_structure = drupal_get_form('unix_time_conversion_timestamp_to_date_form');
      $form = drupal_render($form_structure);
      // Place the form as block content.
      $block['content'] = $form;
      break;

    // Date to timestamp block.
    case 'date_to_unix_timestamp':
      $block['subject'] = '';
      // Get form structure & render the form.
      $form_structure = drupal_get_form('unix_time_conversion_date_to_timestamp_form');
      $form = drupal_render($form_structure);
      // Place the form as block content.
      $block['content'] = $form;
      break;
  }
  // Return the block.
  return $block;
}
