<?php
/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_variable_info().
 */
function dindent_variable_info() {
  $variable['dindent_non_cacheable_pages'] = array(
    'title'   => t('Dindent non-cacheable pages'),
    'type'    => 'boolean',
    'default' => 0,
    'group'   => 'dindent',
  );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function dindent_variable_group_info() {
  $groups['dindent'] = array(
    'title'       => t('Dindent'),
    'description' => t('Dindent configuration settings'),
    'access'      => 'administer site configuration',
    'path'        => 'admin/config/development/dindent',
  );

  return $groups;
}
