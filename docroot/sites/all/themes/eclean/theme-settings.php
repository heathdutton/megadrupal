<?php

/**
 * @file
 * Theme setting callbacks for the eClean theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function eclean_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['eclean_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('eClean settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -10,
  );
  $form['eclean_settings']['eclean_admin_show_sidebars'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show sidebars in admin pages'),
    '#default_value' => theme_get_setting('eclean_admin_show_sidebars'),
    '#description' => t('Check here if you want the theme to show the sidebar regions in the admin pages.'),
    '#weight' => -2,
  );
}

