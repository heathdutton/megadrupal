<?php
/**
 * @file
 * one_ipe.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function one_ipe_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'panelizer_defaults_node_one_site_content';
  $strongarm->value = array(
    'status' => 1,
    'default' => 1,
    'choice' => 0,
  );
  $export['panelizer_defaults_node_one_site_content'] = $strongarm;

  return $export;
}
