<?php

/**
 * @file
 * Entity Print admin form.
 */

/**
 * Implements form callback.
 */
function entity_print_settings_form() {
  $form = array();
  $form['entity_print_default_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Default CSS'),
    '#description' => t('Provides some very basic font and padding styles.'),
    '#default_value' => variable_get('entity_print_default_css', TRUE),
  );
  $form['entity_print_wkhtmltopdf'] = array(
    '#type' => 'textfield',
    '#title' => t('WkhtmlToPdf Location'),
    '#description' => t('Set this to the system path where WkhtmlToPdf is located.'),
    '#default_value' => variable_get('entity_print_wkhtmltopdf', '/usr/local/bin/wkhtmltopdf'),
  );
  return system_settings_form($form);
}
