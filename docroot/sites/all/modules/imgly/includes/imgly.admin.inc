<?php
/**
 * @file
 * Imgly admin pages.
 */

/**
 * Form callback.
 *
 * Returns Imgly settings form.
 *
 * @see imgly_stats_menu().
 */
function imgly_settings_form() {
  $form = array();

  $form['imgly_powered'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "Powered by Imgly" text'),
    '#description' => t('Check this to enable the "Powered by Imgly" text that appears at the bottom of the imgly template.'),
    '#default_value' => variable_get('imgly_powered', 1),
  );

  return system_settings_form($form);
}