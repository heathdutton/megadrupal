<?php
/**
 * @file
 * Variable definitions for Ajax Screen Lock.
 */

/**
 * Implements hook_variable_group_info().
 */
function ajax_screen_lock_variable_group_info() {
  $groups['ajax_screen_lock'] = array(
    'title' => t('Ajax screen lock'),
    'description' => t('Ajax screen lock settings.'),
    'access' => 'administer site configuration',
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function ajax_screen_lock_variable_info($options) {
  $variables['ajax_screen_lock_popup_title'] = array(
    'title' => t('Title of screen lock popup'),
    'group' => 'ajax_screen_lock',
    'localize' => TRUE,
  );

  return $variables;
}
