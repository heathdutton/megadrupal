<?php
/**
 * @file
 * ns_promo.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ns_promo_defaultconfig_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_ns_promo';
  $strongarm->value = array();
  $export['menu_options_ns_promo'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_ns_promo';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_ns_promo'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_ns_promo';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_ns_promo'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_ns_promo';
  $strongarm->value = 0;
  $export['node_submitted_ns_promo'] = $strongarm;

  return $export;
}
