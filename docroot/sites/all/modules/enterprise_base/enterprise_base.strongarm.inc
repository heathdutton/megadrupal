<?php
/**
 * @file
 * enterprise_base.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function enterprise_base_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_day';
  $strongarm->value = 'D, F d';
  $export['date_format_day'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_time';
  $strongarm->value = 'g:i a';
  $export['date_format_time'] = $strongarm;

  return $export;
}
