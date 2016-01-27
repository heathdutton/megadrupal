<?php

/**
 * @file
 * Administration pages for jQuery Scroll Follow.
 */

/**
 * Admin settings.
 */
function jquery_scroll_follow_settings($form_state) {

  $jquery_scroll_follow_path = JQUERY_SCROLL_FOLLOW_DEFAULT_PATH;

  $form['jquery_scroll_follow_css_target'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS selectors'),
    '#description' => t('CSS selectors (separated by a comma)'),
    '#default_value' => variable_get('jquery_scroll_follow_css_target', NULL),
  );

  $form['jquery_scroll_follow_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom settings'),
    '#description' => t('Set your own parameters'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['jquery_scroll_follow_settings']['jquery_scroll_follow_settings_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#description' => t('The duration of the sliding animation (in milliseconds). 
    The smaller the value, the faster the animation. (Default:500)'),
    '#default_value' => variable_get('jquery_scroll_follow_settings_speed', 500),
  );

  $form['jquery_scroll_follow_settings']['jquery_scroll_follow_settings_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Offset'),
    '#description' => t('Number of pixels the Scroll Follow object should 
    remain from top of viewport. (Default:0)'),
    '#default_value' => variable_get('jquery_scroll_follow_settings_offset', 0),
  );

  $form['jquery_scroll_follow_settings']['jquery_scroll_follow_settings_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#description' => t('Time between the end of the scroll and the beginning 
    of the animation in milliseconds. (Default:0)'),
    '#default_value' => variable_get('jquery_scroll_follow_settings_delay', 0),
  );

  $form['jquery_scroll_follow_miscellaneous'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['jquery_scroll_follow_miscellaneous']['jquery_scroll_follow_location'] = array(
    '#type' => 'textfield',
    '#title' => t('jQuery Scroll Follow plugin path'),
    '#default_value' => variable_get('jquery_scroll_follow_location', $jquery_scroll_follow_path),
    '#description' => t('Enter the location of the !download. It is recommended to use e.g. sites/all/libraries/jquery_scroll_follow.',
  array(
    '!download' => l(t('jQuery Scroll Follow plugin'),
  'http://kitchen.net-perspective.com/open-source/scroll-follow/', array('absolute' => TRUE)))),
  );

 $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

 $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('jquery_scroll_follow_settings_reset'),
  );

  return system_settings_form($form);
}

/**
 * Form submit handler; reset to defaults.
 */
function jquery_scroll_follow_settings_reset($form, &$form_state) {

  form_state_values_clean($form_state);

  foreach ($form_state['values'] as $key => $value) {
    variable_del($key);
  }

  drupal_set_message(t('The configuration options have been reset to their default values.'));
}
