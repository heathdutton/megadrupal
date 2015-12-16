<?php
/**
 * @file
 * theme-settings.php
 *
 * Provides theme settings for Bootstrap based themes when admin theme is not.
 *
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function pm_kickstart_theme_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['advanced']['bootstrap_cdn']['bootstrap_bootswatch'] = array(
    '#type' => 'value',
    '#value' => NULL
  );

  $form['components']['navbar']['bootstrap_navbar_position']['#disabled'] = TRUE;
  $form['components']['navbar']['bootstrap_navbar_inverse']['#disabled'] = TRUE;
}
