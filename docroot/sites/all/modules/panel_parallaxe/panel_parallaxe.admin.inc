<?php
/**
 * @file
 * The Admin interface to define module settings.
 */

/**
 * Implements hook_form().
 */
function panel_parallaxe_admin_form($form, &$form_state) {
  $form['panel_parallaxe_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use modules own CSS for panel blocks'),
    '#description' => t('Define the CSS to use to order and arrange panel blocks'),
    '#default_value' => variable_get('panel_parallaxe_css', 1),
  );
  return system_settings_form($form);
}
