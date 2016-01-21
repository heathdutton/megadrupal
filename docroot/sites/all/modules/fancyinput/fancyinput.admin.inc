<?php

/**
 * @file
 * Admin page callbacks for the DRUPAL module.
 */

/**
 * Builds and returns the fancyinput admin settings form.
 *
 * @ingroup forms
 */
function fancyinput_admin_settings() {
  $form['fancyinput_html_tags'] = array(
    '#type' => 'textarea',
    '#title' => t('Specify which HTML elements should be converted. Specify an selector.'),
    '#description' => t('Separated by \',\' for a new element selector'),
    '#default_value' => variable_get('fancyinput_html_tags', '.form-text, .form-textarea'),
  );
  return system_settings_form($form);
}

