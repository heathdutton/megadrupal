<?php
/**
 * @file
 * Views integration for commerce booking flexible pricing.
 */

/**
 * Implements hook_views_data().
 */
function commerce_booking_flexible_pricing_views_data_alter(&$data) {
  $data['field_data_cbt_class_cache']['cbt_class_cache_target_id']['filter'] = array(
    'handler' => 'commerce_booking_flexible_pricing_views_handler_cbt_class_cache',
  );
}
