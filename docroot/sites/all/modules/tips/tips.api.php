<?php

/**
 * API documentation for TIPS module.
 * 
 * @todo, docs
 *
 * hook_tips_info()
 * hook_tips_info_alter()
 * hook_tips_settings_alter()
 * hook_tips_content_alter()
 */

/**
 * Implements hook_tips_info().
 *
 * Define tooltip libraries to be included.
 */
function hook_tips_info() {
  $tips['libraryname'] = array(
    'title' => t('Library Name'),
    'library' => 'libraryname',
    'settings' => array(
      // Default settings used if none are passed.
    ),
    // Pass content as a parameter.
    'content' => 'param',
    // Pass content as the second parameter.
    'content_param' => 2,
    // Please note, only one set of content selectors may be used
    // per library implementation. This means either passing content
    // as a parameter or option.
    'content' => 'option',
    // Passes content to the 'content' option.
    'content_option' => 'content',
  );

  return $tips;
}

/**
 * Implements hook_tips_info_alter().
 *
 * @param(array)
 *  Array of defined tooltip modules with associated
 *  default settings & title.
 */
function hook_tips_info_alter(&$tips) {
  $tips['library']['settings']['setting_key'] = 500;
}

/**
 * Implements hook_tips_settings_alter().
 *
 * @param array
 *   Tooltip settings group & module hook information.
 */
function hook_tips_settings_alter(&$settings) {
  // Alter settings before display.
}

/**
 * Implements hook_tips_content_alter().
 *
 * @param string
 *   Tooltip content to be displayed.
 */
function hook_tips_content_alter(&$content, $tip_id) {
  // Alter content before display.
}
