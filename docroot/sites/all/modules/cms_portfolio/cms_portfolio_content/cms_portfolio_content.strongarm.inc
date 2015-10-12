<?php
/**
 * @file
 * cms_portfolio_content.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function cms_portfolio_content_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_portfolio';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_portfolio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_portfolio';
  $strongarm->value = '1';
  $export['node_preview_portfolio'] = $strongarm;

  return $export;
}
