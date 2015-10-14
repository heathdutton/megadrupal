<?php

/**
 * @file
 * Contains descriptions of hooks and some examples.
 */

/**
 * Declare widget types that can be used with Local Storage.
 *
 * @return array
 *    Widget types.
 */
function hook_local_storage_widget_types() {
  return array(
    'text_textfield' => t('Textfield'),
    'text_textarea' => t('Textarea'),
    'text_textarea_with_summary' => t('Textarea with summary'),
    'number' => t('Number'),
    'options_select' => t('Select'),
  );
}

/**
 * Declare element types that can be used with local_storage.
 *
 * @return array
 *    Element types.
 */
function hook_local_storage_elements() {
  return array(
    'textfield',
    'textarea',
    'text_format',
    'select',
    'checkbox',
    'radio',
  );
}

/**
 * Declare plugins that can be used with Local Storage.
 *
 * @return array
 *    Plugins array.
 */
function hook_local_storage_plugins() {
  return array(
    'local_storage_plain' => array(
      'title' => t('Local Storage Plain Plugin'),
      'elements' => local_storage_get_element_types(),
    ),
  );
}

/**
 * An opportunity to change form element.
 *
 * An opportunity to change form element and Local Storage settings
 * in form element after build callback.
 *
 * @param array $element
 *    Form element.
 * @param array $local_storage
 *    Local Storage settings.
 */
function hook_local_storage_attach_alter(&$element, &$local_storage) {

}

/**
 * Form element "#local_storage" example.
 *
 * This example shows how to use "#local_storage" property
 * in custom forms.
 */
function local_storage_example_form($form, &$form_state) {
  $form['text'] = array(
    '#type' => 'textfield',
    // Use default settings if needed.
    // '#local_storage' => TRUE,

    // Override default settings with custom ones.
    '#local_storage' => array(
      // Show origin (default) value by default.
      'default' => TRUE,
      'plugins' => array(
        // Enable plain plugin.
        'local_storage_plain' => TRUE,
        // Disable message plugin.
        'local_storage_message' => FALSE,
      ),
      // Set expiration time to 24 hrs.
      'expire' => 24,
    ),
  );

  return $form;
}
