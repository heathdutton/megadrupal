<?php

/**
 * @file
 * Administrative interface to clone a field within same content type.
 */

/**
 * Form builder for the cloning a field within content type.
 *
 * @param string $entity_type
 *   The machine name of the entity.
 * @param string $bundle_name
 *   The machine name of the bundle, or a bundle object if the particular
 *   entity type has a menu loader for bundles.
 */
function quick_field_clone_field_form($form, &$form_state, $entity_type, $bundle_name) {
  // Get the bundle name if the bundle name is really a bundle object.
  $bundle_name = field_extract_bundle($entity_type, $bundle_name);

  $field_instances = field_info_instances($entity_type, $bundle_name);

  $options_fields = array();
  foreach ($field_instances as $field_name => $field) {
    $options_fields[$field_name] = $field['label'];
  }

  $form['fields'] = array(
    '#title' => t('Fields to clone'),
    '#type' => 'radios',
    '#options' => $options_fields,
    '#description' => t("Select fields to clone onto this content type."),
    '#required' => TRUE,
  );

  $form['clone_name'] = array(
    '#title' => t('New field Name'),
    '#type' => 'textfield',
    '#description' => t("New Field Name."),
    '#required' => TRUE,
    '#maxlength' => 26,
    '#size' => 26,
  );

  $form['field_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('New Machine Name'),
    '#title_display' => 'invisible',
    '#field_prefix' => '<span dir="ltr">field_',
    '#field_suffix' => '</span>&lrm;',
    '#size' => 15,
    '#description' => t('A unique machine-readable name containing letters, numbers, and underscores.'),
    // 32 characters minus the 'field_' prefix.
    '#maxlength' => 26,
    '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
    '#machine_name' => array(
      'source' => array('clone_name'),
      'exists' => '_quick_field_field_name_exists',
      'standalone' => TRUE,
      'label' => '',
    ),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Clone Field'),
  );

  return $form;
}

/**
 * Form validation handler for quick_field_clone_field_form().
 */
function quick_field_clone_field_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $field_name = 'field_' . $values['clone_name'];
  if (field_read_fields(array('field_name' => $field_name), array('include_inactive' => TRUE))) {
    form_set_error($values['clone_name'], t('Already Exists.'));
  }
}

/**
 * Form validation handler for quick_field_clone_field_form().
 */
function quick_field_clone_field_form_submit($form, &$form_state) {
  list($entity_type, $source_bundle_name) = $form_state['build_info']['args'];
  // Get names of fields to clone.
  $values = $form_state['values'];
  $field_name = $form_state['values']['fields'];
  $field = field_info_field($field_name);
  $bundle_type = $source_bundle_name->type;
  $new_instance = field_info_instance($entity_type, $field_name, $bundle_type);
  $new_field_name = "field_" . $values['field_name'];
  $new_field = array(
    'field_name' => $new_field_name,
    'type' => $field['type'],
    'translatable' => $field['translatable'],
    'settings' => $field['settings'],
  );
  $new_instance_values = array(
    'field_name' => $new_field_name,
    'label' => $values['clone_name'],
  );
  $new_instance = array_merge($new_instance, $new_instance_values);
  try {
    field_create_field($new_field);
    field_create_instance($new_instance);
    drupal_set_message(t('Field Cloned'));
  }
  catch (Exception $e) {
    drupal_set_message(t('Error Message: %message.', array('%message' => $e->getMessage())));
  }
}
