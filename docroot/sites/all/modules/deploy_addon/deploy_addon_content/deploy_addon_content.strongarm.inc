<?php
/**
 * @file
 * deploy_addon_content.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function deploy_addon_content_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'deploy_adhoc_plan_name';
  $strongarm->value = 'push_to_next';
  $export['deploy_adhoc_plan_name'] = $strongarm;

  return $export;
}
