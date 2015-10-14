<?php

/**
 * @file
 * Functionality needed by the Strip 4-byte UTF8 admin pages.
 */

/**
 * System form for Strip 4-byte UTF8 configuration settings.
 */
function strip_utf8mb4_configuration_form($form, &$form_state) {
  $text_field_widget_types = array(
    'text_textfield' => t('Text'),
    'text_textarea' => t('Long text'),
    'text_textarea_with_summary' => t('Long text and summary'),
    'core_title' => t('Core node\'s title'),
  );
  
  $form['strip_utf8mb4_intro'] = array(
    '#type' => 'markup',
    '#markup' => '<h4>' . t('Configure text fields widget types to reject overly long 2 byte sequences, as well as characters above U+10000, and reject overly long 3 byte sequences and UTF-16.') . '</h4>',
  );
  $form['strip_utf8mb4_for_text_field_widget_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Strip 4-byte UTF8 characters for the following text field widget types'),
    '#options' => $text_field_widget_types,
    '#default_value' => variable_get('strip_utf8mb4_for_text_field_widget_types', array_keys($text_field_widget_types)),
  );

  if (module_exists('webform')) {
    $webform_component_types = array(
      'textarea' => t('Textarea'),
      'textfield' => t('Textfield'),
    );
    $form['webform_strip_utf8mb4_for_component_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Strip 4-byte UTF8 characters for the following webform component types'),
      '#options' => $webform_component_types,
      '#default_value' => variable_get('webform_strip_utf8mb4_for_component_types', array_keys($webform_component_types)),
    );
  }
  
  $form['strip_utf8mb4_replace_string'] = array(
    '#type' => 'textfield',
    '#title' => t('Strip 4-byte UTF8 and replace them with this string'),
    '#description' => t('Change the replace string if you do not want to use  the default "--" .'),
    '#default_value' => variable_get('strip_utf8mb4_replace_string', '--'),
  );

  return system_settings_form($form);
}
