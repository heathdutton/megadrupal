<?php
/**
 * @file
 * Page callback: Autofill forms configuration settings.
 */

/**
 * Admin configuration form.
 */
function autofill_forms_configure_admin_form($form, &$form_state) {
  $form['autofill'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Autofill Form Option'),
  );

  // Form Field Permission.
  $form['autofill']['permission'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Form Field Permission'),
    '#description' => t('Please select which field you want fill default value.'),
  );

  $field_options = array(
    'text' => 'Text Field',
    'textarea' => 'Textarea',
    'checkbox' => 'Checkbox',
    'radio' => 'Radio Button',
    'selectbox' => 'Select Box',
  );
  foreach ($field_options as $option_type => $option_name) {
    $form['autofill']['permission']['autofill_forms_using_fields_perm_' . $option_type] = array(
      '#type' => 'checkbox',
      '#title' => $option_name,
      '#default_value' => variable_get('autofill_forms_using_fields_perm_' . $option_type) != NULL ? 1 : 0,
    );
  }

  // Field words count setting.
  $form['autofill']['field_length'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Form Field Setting'),
    '#description' => t('Please enter how many words create default value.'),
  );
  $form['autofill']['field_length']['autofill_forms_using_text_field_length'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Text'),
    '#default_value' => variable_get('autofill_forms_using_text_field_length'),
    '#description' => t('Enter textfield default value words. If not required, leave blank.'),
  );
  $form['autofill']['field_length']['autofill_forms_using_textarea_field_length'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Textarea'),
    '#default_value' => variable_get('autofill_forms_using_textarea_field_length'),
    '#description' => t('Enter textarea default value words. If not required, leave blank.'),
  );

  // Setting Content type.
  $form['autofill']['permissions'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Content Type Permissions'),
  );
  $form['autofill']['permissions']['autofill_forms_using_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select Content Types'),
    '#description' => t('Enable Autofill form generation for the following content types'),
    '#default_value'  => variable_get('autofill_forms_using_node_types', array()),
    '#options' => node_type_get_names(),
  );

  return system_settings_form($form);
}
