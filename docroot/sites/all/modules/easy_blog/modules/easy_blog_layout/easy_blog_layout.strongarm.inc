<?php
/**
 * @file
 * easy_blog_layout.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function easy_blog_layout_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_blog_post_pattern';
  $strongarm->value = 'blog/[current-date:custom:Y]/[current-date:custom:m]/[node:title]';
  $export['pathauto_node_blog_post_pattern'] = $strongarm;

  return $export;
}
