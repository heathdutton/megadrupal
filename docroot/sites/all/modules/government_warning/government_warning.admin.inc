<?php
/**
 * @file
 * Admin settings for Government warning.
 */

/**
 * Admin settings form.
 */
function government_warning_admin_settings() {
  $warning_text = variable_get('government_warning_text', NULL);

  $form = array();
  $form['government_warning_text'] = array(
    '#title' => t('Warning text for display on login pages'),
    '#type' => 'text_format',
    '#format' => 'filtered_html',
    '#required' => TRUE,
    '#default_value' => $warning_text ? $warning_text['value'] : $warning_text,
  );

  return system_settings_form($form);
}
