<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function austin_form_system_theme_settings_alter(&$form, &$form_state)  {

  // Create the form using Forms API: http://api.drupal.org/api/7

  $form['austin_color_title'] = array(
    '#type' => 'item',
    '#title' => t('Color palette'),
  );
  $form['austin_color_option'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display the alternate blue/red color palette.'),
    '#default_value' => theme_get_setting('austin_color_option'),
  );

  // Remove some of the base theme's settings.
  unset($form['themedev']['zen_layout']); // We don't need to select the base stylesheet.

  // We are editing the $form in place, so we don't need to return anything.
}
