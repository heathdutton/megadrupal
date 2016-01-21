<?php
/**
 * @file
 * Contact Info variable definitions using the Variable API.
 */

/**
 * Implements hook_variable_info().
 */
function contactinfo_variable_info($options) {
  $variable['contactinfo'] = array(
    'title' => t('Contact information'),
    'description' => t('Contact information for this website.'),
    'type' => 'array',
    'default callback' => 'contactinfo_defaults',
    'group' => 'contactinfo',
    'localize' => TRUE,
  );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function contactinfo_variable_group_info() {
  $groups['contactinfo'] = array(
    'title' => t('Contact information'),
    'description' => t('Contact Information'),
    'access' => 'administer contactinfo',
    'path' => 'admin/config/system/contactinfo',
  );
  return $groups;
}
