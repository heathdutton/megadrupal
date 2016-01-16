<?php

/**
 * @file
 * Theme setting callbacks for the PGN responsive theme.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function willibald_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL)  {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }

  $form['theme_settings']['toggle_tertiary_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tertiary menu'),
    '#default_value' => theme_get_setting('toggle_tertiary_menu'),
  );

  $form['menu_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu settings'),
    '#weight' => -1,
  );

  $form['menu_settings']['main_menu_home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show home button in main menu.'),
    '#default_value' => theme_get_setting('main_menu_home'),
  );

  $form['menu_settings']['tertiary_menu'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary menu'),
    '#default_value' => theme_get_setting('tertiary_menu'),
    '#options' => menu_get_menus(),
  );

  $form['theme_settings'] += array(
    '#weight' => -2,
  );

  return $form;

}
