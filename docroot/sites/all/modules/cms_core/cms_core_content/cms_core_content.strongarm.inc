<?php
/**
 * @file
 * cms_core_content.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function cms_core_content_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_block_1_level';
  $strongarm->value = '2';
  $export['menu_block_1_level'] = $strongarm;

  return $export;
}
