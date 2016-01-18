<?php

/**
 * @file
 * Provides integration with rules module
 */

/**
 * Implements hook_rules_action_info().
 */
function resource_booking_rules_action_info() {
  $arg = array(
    'type' => 'node',
    'label' => t('Resource booking node'),
  );

  $actions['resource_booking_update_status'] = array(
    'label' => t('Update the status of resource booking'),
    'group' => t('Resource Booking'),
    'parameter' => array(
      'node' => $arg,
      'resource_booking_status' => array(
        'type' => 'text',
        'label' => t('Status'),
        'options list' => '_resource_booking_field_get_rb_status',
      ),
    ),
  );

  return $actions;
}
