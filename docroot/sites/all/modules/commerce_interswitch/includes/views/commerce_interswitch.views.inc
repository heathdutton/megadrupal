<?php
/**
 * @file
 * Views integration for Commerce Interswitch.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_interswitch_views_data_alter(&$data) {
  $data['commerce_payment_transaction']['get_status'] = array(
    'field' => array(
      'title' => t('Update from Interswitch'),
      'help' => t('Get the status of the payment from Interswitch'),
      'handler' => 'commerce_interswitch_field_handler_get_status',
    ),
  );
}
