<?php
/**
 * @file
 * restaurant_pages.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function restaurant_pages_defaultconfig_features() {
  return array(
    'restaurant_pages' => array(
      'strongarm' => 'strongarm',
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_strongarm().
 */
function restaurant_pages_defaultconfig_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'panels_everywhere_override_theme_restaurant_radix';
  $strongarm->value = 0;
  $export['panels_everywhere_override_theme_restaurant_radix'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'panels_everywhere_provide_sample';
  $strongarm->value = 0;
  $export['panels_everywhere_provide_sample'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'panels_everywhere_site_template_enabled';
  $strongarm->value = 1;
  $export['panels_everywhere_site_template_enabled'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'panels_everywhere_site_template_enabled_admin';
  $strongarm->value = 0;
  $export['panels_everywhere_site_template_enabled_admin'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'panels_everywhere_site_template_per_theme';
  $strongarm->value = 0;
  $export['panels_everywhere_site_template_per_theme'] = $strongarm;

  return $export;
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function restaurant_pages_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access user contact forms'.
  $permissions['access user contact forms'] = array(
    'name' => 'access user contact forms',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'contact',
  );

  // Exported permission: 'administer contact forms'.
  $permissions['administer contact forms'] = array(
    'name' => 'administer contact forms',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'contact',
  );

  return $permissions;
}
