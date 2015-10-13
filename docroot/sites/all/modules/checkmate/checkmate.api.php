<?php
/**
 * @file
 * Checkmate api overview.
 */

/**
 * Implements hook_checkmate_check_callbacks().
 */
function hook_checkmate_check_callbacks() {
  $items = array();
  $items['checkmate_is_module_enabled'] = array(
    'label' => t('Is module enabled'),
    'callback' => 'checkmate_is_module_enabled',
    'arguments' => array(
      'checkmate_module_name' => array(
        '#type' => 'textfield',
        '#title' => t('Module'),
      ),
    ),
  );
  $items['checkmate_is_cron_enabled'] = array(
    'label' => t('Is cron enabled'),
    'callback' => 'checkmate_is_cron_enabled',
  );
  return $items;
}
