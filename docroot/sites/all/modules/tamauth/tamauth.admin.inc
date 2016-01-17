<?php
/**
 * @file
 *  Tamauth module admin functions.
 */

define('TAM_HEADER_FIELD_MAPPING_PATTERN', '/(.+)=(.+)/');

/**
 * Page callback for admin/config/tamauth.
 */
function tamauth_settings_form($form, $form_state) {
  $mappings = tamauth_field_mappings();
  $mappings_text = '';

  foreach ($mappings as $tam_header => $drupal_field_name) {
    $mappings_text .= $tam_header . '=' . $drupal_field_name . "\n";
  }

  $form['tam_field_mappings'] = array(
    '#title' => t('Header / Field Mappings'),
    '#type' => 'textarea',
    '#description' => t('Enter your header to field mappings, one per line in the format:') . '<br />' . t('TAM_HEADER=drupal_field_name'),
    '#default_value' => $mappings_text,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 10,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save configuration',
  );

  return $form;
}

/**
 * Validate handler for tamauth_settings_form.
 */
function tamauth_settings_form_validate($form, $form_state) {
  $mappings = trim($form_state['values']['tam_field_mappings']);
  if (!empty($mappings) && !preg_match(TAM_HEADER_FIELD_MAPPING_PATTERN, $mappings)) {
    form_set_error('tam_field_mappings', t('Header / Field Mappings invalid'));
  }
}

/**
 * Submit handler for tamauth_settings_form.
 */
function tamauth_settings_form_submit($form, $form_state) {
  $mappings = trim($form_state['values']['tam_field_mappings']);
  $tam_field_mappings = array();
  if (preg_match_all(TAM_HEADER_FIELD_MAPPING_PATTERN, $mappings, $matches)) {
    $tam_headers = array_map('trim', $matches[1]);
    $drupal_field_names = array_map('trim', $matches[2]);
    $tam_field_mappings = array_combine($tam_headers, $drupal_field_names);
  }
  variable_set('tam_field_mappings', $tam_field_mappings);
}
