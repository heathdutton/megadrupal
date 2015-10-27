<?php
/**
 * @file
 * Implements hooks for variable api module.
 */

/**
 * Implements hook_variable_group_info().
 */
function panopoly_config_variable_group_info() {
  $groups = array();
  $panopoly_config = _panopoly_config_get_config();

  // Create variable group from config.
  if (count($panopoly_config)) {
    foreach ($panopoly_config as $group_name => $group_info) {
      $variable_group_name = _panopoly_config_get_config_variable_group_name($group_name);
      $groups[$variable_group_name] = $group_info;
    }
  }

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function panopoly_config_variable_info($options) {
  $variables = array();
  $panopoly_config = _panopoly_config_get_config();

  // Create variables from config.
  if (count($panopoly_config)) {
    foreach ($panopoly_config as $group_name => $group_info) {
      if (isset($group_info['config'])) {
        foreach ($group_info['config'] as $config_name => $config_info) {
          $variable_name = _panopoly_config_get_config_variable_name($config_name);
          $variables[$variable_name] = $config_info;

          // Add group.
          $variable_group_name = _panopoly_config_get_config_variable_group_name($group_name);
          $variables[$variable_name]['group'] = $variable_group_name;
        }
      }
   }
  }

  return $variables;
}