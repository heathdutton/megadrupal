<?php
/**
 * @file
 * ns_blog.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ns_blog_defaultconfig_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_ns_blog';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_ns_blog'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_ns_blog';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_ns_blog'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_ns_blog';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_ns_blog'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_ns_blog';
  $strongarm->value = 1;
  $export['node_submitted_ns_blog'] = $strongarm;

  return $export;
}
