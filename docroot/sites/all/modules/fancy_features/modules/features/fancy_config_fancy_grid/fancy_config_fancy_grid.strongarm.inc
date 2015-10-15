<?php
/**
 * @file
 * fancy_config_fancy_grid.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fancy_config_fancy_grid_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'fancy_grid_breakpoint_group';
  $strongarm->value = 'fancy_grid_default';
  $export['fancy_grid_breakpoint_group'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'fancy_grid_compile';
  $strongarm->value = '1';
  $export['fancy_grid_compile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'fancy_grid_compile_webservice';
  $strongarm->value = 'http://sassmeister.com/app/3.3/compile';
  $export['fancy_grid_compile_webservice'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'fancy_grid_grid_max_width';
  $strongarm->value = '1180px';
  $export['fancy_grid_grid_max_width'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'fancy_grid_grid_width';
  $strongarm->value = '100%';
  $export['fancy_grid_grid_width'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'fancy_grid_layouts';
  $strongarm->value = array(
    'custom.user.mobile' => array(
      'breakpoint' => '(width 0 480px)',
      'identifier' => 'mobile',
      'columns' => '12',
      'gutters' => '2%',
      'config' => '1|12
1,7|6
1,5,9|4',
    ),
    'custom.user.tablet' => array(
      'breakpoint' => '(width 481px 768px)',
      'identifier' => 'tablet',
      'columns' => '12',
      'gutters' => '2%',
      'config' => '1|12
1,7|6
1,5,9|4
1,4,7,10|3',
    ),
    'custom.user.desktop' => array(
      'breakpoint' => '(min-width 769px)',
      'identifier' => 'desktop',
      'columns' => '12',
      'gutters' => '2%',
      'config' => '1|12
1,7|6
1,5,9|4
1,4,7,10|3',
    ),
  );
  $export['fancy_grid_layouts'] = $strongarm;

  return $export;
}
