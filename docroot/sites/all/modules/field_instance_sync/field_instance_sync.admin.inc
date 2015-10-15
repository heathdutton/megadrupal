<?php

/**
 * @file
 * Administration interface for the Field Instance Sync module.
 */

/**
 * Form builder; Creates the settings form.
 */
function field_instance_sync_settings_form($form, &$form_state) {
  // Compile a list of fieldable entities.
  $entities = array();
  foreach (entity_get_info() as $type => $info) {
    if (!empty($info['fieldable'])) {
      $entities[$type] = check_plain($info['label']);
    }
  }

  asort($entities);

  $form['field_attach_entities'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Automatically attach fields'),
    '#description' => t('Automatically attach all synced fields to new bundles that are created of the selected entity types.'),
    '#options' => $entities,
    '#default_value' => variable_get('field_instance_sync_field_attach_entities', array()),
  );

  // Compile a list of synced field instances and order them by entity type and
  // bundle.
  $data = array();
  $bundles = field_info_bundles();
  foreach (field_instance_sync_get_synced_instances() as $instance) {
    $label = $bundles[$instance['entity_type']][$instance['bundle']]['label'];
    $data[$instance['entity_type']][$instance['field_name']][$instance['bundle']] = $label;
  }

  // Define a vertical tabs group.
  $form['entity_types'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['master_bundles'] = array(
    '#tree' => TRUE,
  );
  $default_values = variable_get('field_instance_sync_master_bundles', array());

  // Generate a vertical tab for each entity type.
  foreach ($data as $entity_type => $fields) {
    $entity_info = entity_get_info($entity_type);
    $form['master_bundles'][$entity_type] = array(
      '#type' => 'fieldset',
      '#title' => $entity_info['label'],
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#group' => 'entity_types',
      'fields' => array(
        '#type' => 'fieldset',
        '#title' => t('Master bundle'),
      ),
    );

    // Generate select boxes to choose the master bundle for all fields.
    ksort($fields);
    foreach ($fields as $field_name => $bundles) {
      $form['master_bundles'][$entity_type]['fields']['fields'][$field_name] = array(
        '#type' => 'select',
        // The machine name is used as the title, rather than the human readable
        // label. The label is not unique and can be different for each field
        // instance, leading to potential confusion.
        '#title' => $field_name,
        '#options' => $bundles,
        '#default_value' => !empty($default_values[$entity_type][$field_name]) ? $default_values[$entity_type][$field_name] : NULL,
      );
    }
  }

  // Provide a submit button.
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array('#type' => 'submit', '#value' => t('Save configuration')),
  );

  return $form;
}

/**
 * Form submission handler for field_instance_sync_settings_form().
 */
function field_instance_sync_settings_form_submit($form, &$form_state) {
  // Save the field attach settings.
  variable_set('field_instance_sync_field_attach_entities', $form_state['values']['field_attach_entities']);

  // Save the master bundles settings.
  $master_bundles = array();
  if (!empty($form_state['values']['master_bundles'])) {
    foreach ($form_state['values']['master_bundles'] as $entity_type => $fields) {
      $master_bundles[$entity_type] = $fields['fields']['fields'];
    }
  }
  variable_set('field_instance_sync_master_bundles', $master_bundles);

  drupal_set_message(t('The configuration options have been saved.'));
}
