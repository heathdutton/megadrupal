<?php

/**
 * @file
 * Variable integration for Path metatags module.
 */

/**
 * Implements hook_variable_info().
 */
function path_metatags_variable_info($options) {
  $variables = array();

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function path_metatags_variable_group_info() {

  $groups['metatags'] = array(
    'title' => t('Path metatags'),
    'description' => t('Variables for Path metatags configuration.'),
    'access' => 'administer path metatags',
    'path' => array('admin/structure/path-metatags/settings'),
  );

  return $groups;
}
