<?php
/**
 * @file
 * Provides infrequently used functions and hooks for normalize.
 */

/**
 * Menu callback: admin settings form.
 *
 * @return
 *   The settings form used by Normalize.
 */
function normalize_admin_settings_form($form, &$form_state) {
  // Option to toggle the version of Normalize.css
  $form['normalize_version'] = array(
    '#type' => 'radios',
    '#title' => t('Choose which Normalize.css to use'),
    '#default_value' => variable_get('normalize_version', '3'),
    '#options' => array(
      '3' => t('Version 3 without support for IE 6/7'),
      '1' => t('Version 1 with support for IE 6/7'),
    ),
    '#description' => t('Normalize version 3 is a simpler stylesheet because it lacks support for the older IE 6/7 browsers.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission handler.
 */
function normalize_admin_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['normalize_version'] === '1') {
    variable_set('normalize_version', '1');
  }
  else {
    variable_del('normalize_version');
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Implements hook_help().
 */
function _normalize_help($path, $arg) {
  switch ($path) {
    case 'admin/help#normalize':
      $output = '<h3>' . t('Usage') . '</h3>';
      $output .= '<p>' . t('Nicolas Gallagher explains <a href="http://nicolasgallagher.com/about-normalize-css/">two approaches to use normalize.css</a> on your website. This project supports both methods.') . '</p>';
      $output .= '<ol><li>';
      $output .= '<blockquote style="margin-left: 0; margin-right: 0; font-weight: bold">' . t('"Include normalize.css untouched and build upon it, overriding the defaults later in your CSS if necessary."') . '</blockquote>';
      $output .= '<p>' . t('To use this approach, simply enable the Normalize module and override what you\'d like in your theme.') . '</p>';
      $output .= '</li><li>';
      $output .= '<blockquote style="margin-left: 0; margin-right: 0; font-weight: bold">' . t('"Use normalize.css as a starting point for your own project\'s base CSS, customising the values to match the design\'s requirements."') . '</blockquote>';
      $output .= '<p>' . t('To use this approach, copy the normalize.css/normalize-rtl.css or normalize.scss/normalize-rtl.scss files into your theme. You can then add them and modify them like normal theme stylesheets.') . '</p>';
      $output .= '<p>' . t('You can optionally use this module so that the normalize.css stylesheet comes before all other stylesheets. As long as your theme\'s version is still named "normalize.css", your version will be loaded instead of the module\'s original version.') . '</p>';
      $output .= '</li></ol>';
    return $output;
  }
}
