<?php
/**
 * @file
 * Admin settings function for fartscroll.
 */

/**
 *  Admin settings configuration.
 */
function fartscroll_admin_settings_form($form_state) {
  $form['fartscroll_pages'] = array(
    '#type' => 'textarea',
    '#title' => 'Fartscroll pages',
    '#default_value' => variable_get('fartscroll_pages', ''),
    '#description' => t('List the paths on which you want fart sounds audible during scrolling. Enter one path per line. The "*" character is a wildcard. (Defaults to all pages.)'),
    '#rows' => 10,
  );
  $form['fartscroll_farts'] = array(
    '#type' => 'textfield',
    '#title' => t('Pixels per fart'),
    '#default_value' => variable_get('fartscroll_farts', 400),
    '#maxlength' => 255,
    '#size' => 45,
    '#description' => t('Specify the number of pixels a user must scroll to receive a fart.'),
  );

  return system_settings_form($form);
}
