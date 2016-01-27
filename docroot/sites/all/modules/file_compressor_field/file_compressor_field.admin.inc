<?php

/**
 * @file
 * Administrative page callbacks for File Compressor_field module.
 */

/**
 * Rebuild File Compressor field items form handler.
 */
function file_compressor_field_settings_form($form, &$form_state) {
  $fields = array_filter(field_info_field_map(), function($item) {
    return $item['type'] == 'file_compressor';
  });
  $entity_info = entity_get_info();
  $options = array();
  foreach ($fields as $field_name => $field) {
    foreach ($field['bundles'] as $entity_type_name => $bundles) {
      $entity_type = $entity_info[$entity_type_name];
      foreach ($bundles as $bundle_name) {
        $bundle = $entity_type['bundles'][$bundle_name];
        $instance = field_info_instance($entity_type_name, $field_name, $bundle_name);
        $option_key = implode('|', array($entity_type_name, $bundle_name));
        if (!isset($options[$option_key])) {
          $options[$option_key] = array($instance['label'], $entity_type['label'], $bundle['label']);
        }
        else {
          $options[$option_key][0] = $options[$option_key][0] . ', ' . $instance['label'];
        }
      }
    }
  }

  $headers = array(t('Field Name'), t('Entity type'), t('Bundle'));
  $form['rebuild'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $options,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 50,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Field(s)'),
  );

  return $form;
}

/**
 * Validate callback for Rebuild File Compressor field items form handler.
 */
function file_compressor_field_settings_form_validate($form, &$form_state) {
  $selected_instances = array_filter($form_state['values']['rebuild']);
  if (empty($selected_instances)) {
    form_set_error('rebuild', t('Please select at least one field to update.'));
  }
}

/**
 * Submit callback for Rebuild File Compressor field items form handler.
 */
function file_compressor_field_settings_form_submit($form, &$form_state) {
  $instances = array_filter($form_state['values']['rebuild']);
  foreach ($instances as $instance) {
    list($entity_type_name, $bundle_name) = explode('|', $instance);
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type_name)
      ->entityCondition('bundle', $bundle_name);
    $result = $query->execute();
    $ids = array_keys($result[$entity_type_name]);
    $entities = entity_load($entity_type_name, $ids);
    foreach ($entities as $entity) {
      field_attach_presave($entity_type_name, $entity);
      field_attach_update($entity_type_name, $entity);
    }
  }
}
