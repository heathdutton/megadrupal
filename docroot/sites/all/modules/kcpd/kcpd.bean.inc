<?php
/**
 * @file
 * kcpd.bean.inc
 */

/**
 * Implements hook_bean_admin_ui_types().
 */
function kcpd_bean_admin_ui_types() {
  $export = array();

  $bean_type = new stdClass();
  $bean_type->disabled = FALSE; /* Edit this to true to make a default bean_type disabled initially */
  $bean_type->api_version = 5;
  $bean_type->name = 'promo';
  $bean_type->label = 'Promo';
  $bean_type->options = '';
  $bean_type->description = 'Add a new promo for a shop.';
  $export['promo'] = $bean_type;

  return $export;
}
