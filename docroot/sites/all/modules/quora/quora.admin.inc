<?php
/**
 * @file
 * Contains configuration settings for Quora module.
 */

/**
 * Callback to admin/config/quora.
 */
function quora_config_form($form, &$form_state) {
  $form = array();
  $form['quora_google_cse_api'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Custom Search Api'),
    '#description' => t('Provide google cse api to be used my module'),
    '#default_value' => variable_get('quora_google_cse_api', NULL),
  );
  $form['quora_google_cse_cx'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Custom Search Engine CX ID'),
    '#default_value' => variable_get('quora_google_cse_cx', NULL),
    '#description' => t('The custom search engine corresponding to this cx-id must be able to search quora.com'),
  );
  $form['quora'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Tags field for content types.'),
    '#description' => t('This field content will be used for fetching related quora questions.'),
  );
  foreach (node_type_get_types() as $content_type) {
    $fields = field_info_instances("node", $content_type->type);
    if ($fields) {
      $options = array();
      foreach ($fields as $field) {
        $options[$field['field_name']] = $field['label'];
      }
      $form['quora']['quora_' . $content_type->type . '_field'] = array(
        '#type' => 'select',
        '#title' => $content_type->name,
        '#options' => array_merge(array('0' => t('Auto')), $options),
        '#default_value' => variable_get('quora_' . $content_type->type . '_field', 'field_tags'),
      );
    }
  }
  return system_settings_form($form);
}
