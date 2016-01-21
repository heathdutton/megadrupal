<?php
/**
 * @file
 * Plugin to provide access control based on mobile device detection.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Mobile Switch: Mobile detection'),
  'description' => t('Controls access by depending on mobile device detection.'),
  'callback' => 'mobile_switch_panels_device_ismobile_ctools_access_check',
  'default' => array('device_ismobile' => 0),
  'settings form' => 'mobile_switch_panels_device_ismobile_ctools_settings',
  'summary' => 'mobile_switch_panels_device_ismobile_ctools_summary',
  'all contexts' => TRUE
);

/**
 * Custom callback defined by 'callback' in the $plugin array.
 *
 * Check for access.
 */
function mobile_switch_panels_device_ismobile_ctools_access_check($conf, $context) {
  $detect = mobile_switch_mobile_detect();

  $ismobile = $detect['ismobiledevice'] ? '1' : '0';

  if ($detect['tablet_usage'] === FALSE) {
    return TRUE;
  }

  // Check theme cookie exists or a theme switch action by visitor.
  // Theme changed means: Mobile site <-> standard site
  $theme_switch = mobile_switch_panels_check_theme_switch();

  if (!$theme_switch) {
    if ($conf['device_ismobile'] != $ismobile) {
      return FALSE;
    }
  }
  // Theme cookie exists or a visitor has changed the theme.
  else {
    switch ($conf['device_ismobile']) {
      case '0':
        if ($ismobile == '1' && $theme_switch == 'mobile') {
          return FALSE;
        }
        break;
      case '1':
        if ($ismobile == '1' && $theme_switch == 'standard') {
          return FALSE;
        }
        break;
    }
  }

  return TRUE;
}

/**
 * Settings form for the 'device_ismobile' access plugin.
 */
function mobile_switch_panels_device_ismobile_ctools_settings(&$form, &$form_state, $conf) {
  $form['settings']['device_ismobile'] = array(
    '#type' => 'select',
    '#title' => t('Display only'),
    '#default_value' => $conf['device_ismobile'],
    '#options' => mobile_switch_panels_ismobile_options()
  );

  return $form;
}

/**
 * Provide a summary description based upon the age.
 */
function mobile_switch_panels_device_ismobile_ctools_summary($conf, $context) {
  $options = mobile_switch_panels_ismobile_options();

  return t('Detected device @ismobile', array('@ismobile' => lcfirst($options[$conf['device_ismobile']])));
}
