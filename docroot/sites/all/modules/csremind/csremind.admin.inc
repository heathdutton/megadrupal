<?php

/**
 * @file
 * Module administration functionality.
 */

/**
 * Module settings form.
 */
function csremind_settings($form, &$form_state) {
  $form = array();

  // @todo Add ability for user to add own frequency values
  $form['csremind_default_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Default reminder frequency'),
    '#options' => drupal_map_assoc(
      array(
        0,
        30 ,
        300,
        900,
        1800,
        3600,
        7200,
        14400,
        28800,
        57600,
        86400,
      ),
      'format_interval'
    ),
    '#default_value' => variable_get('csremind_default_frequency', 1800),
    '#description' => t('Default value used when a new reminder record is
      created.'),
  );

  $form['csremind_exclude_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages to exclude reminders triggering on'),
    '#default_value' => variable_get('csremind_exclude_pages', CSREMIND_EXCLUDE_PAGES),
    '#cols' => 40,
    '#rows' => 5,
    '#description' => t("Enter one page per line as Drupal paths. The '*'
      character is a wildcard. Example paths are '<em>blog</em>' for the main
      blog page and '<em>blog/*</em>' for every personal blog."),
  );

  return system_settings_form($form);
}
