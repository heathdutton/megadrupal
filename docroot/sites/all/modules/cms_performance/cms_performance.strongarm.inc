<?php
/**
 * @file
 * cms_performance.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function cms_performance_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'css_emimage_force_inline';
  $strongarm->value = 1;
  $export['css_emimage_force_inline'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'css_emimage_ielimit';
  $strongarm->value = 1;
  $export['css_emimage_ielimit'] = $strongarm;

  return $export;
}
