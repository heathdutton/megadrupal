<?php

/**
 * @file
 * Makes text-data in fields unreadable.
 */

/**
 * Select entity types.
 */
function shredder_step_1($form, &$form_state) {
  $number_of_rows = db_select('shredder_changed_entities')->range(0, 1)->countQuery()->execute()->fetchField();
  if ($number_of_rows > 0) {
    $form['clear_message'] = array(
      '#markup' => '<div class="messages warning">' . t('Changed entities will be ignored. Clear result table to shredder all entities.') . '</div>',
    );
    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear results table'),
      '#limit_validation_errors' => array(),
      '#submit' => array('shredder_clear_results_table'),
    );
  }
  $form['warning'] = array(
    '#markup' => '<div class="messages error">' . t('All data from selected fields will be destroyed!') . '</div>',
  );
  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity types'),
    '#options' => shredder_get_entity_types(),
    '#default_value' => !empty($form_state['object']['entity_type']) ? $form_state['object']['entity_type'] : '',
    '#multiple' => TRUE,
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Select bundles.
 */
function shredder_step_2($form, &$form_state) {

  $form['bundles'] = array(
    '#type' => 'select',
    '#title' => t('Bundles'),
    '#options' => shredder_get_bundles($form_state),
    '#default_value' => !empty($form_state['object']['bundles']) ? $form_state['object']['bundles'] : '',
    '#multiple' => TRUE,
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Select field types.
 */
function shredder_step_3($form, &$form_state) {

  $form['field_type'] = array(
    '#type' => 'select',
    '#title' => t('Field types'),
    '#options' => shredder_get_field_types(),
    '#default_value' => !empty($form_state['object']['field_type']) ? $form_state['object']['field_type'] : '',
    '#multiple' => TRUE,
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Select fields.
 */
function shredder_step_4($form, &$form_state) {
  $form['fields'] = array(
    '#type' => 'select',
    '#title' => t('Fields'),
    '#options' => shredder_get_fields($form_state),
    '#default_value' => !empty($form_state['object']['fields']) ? $form_state['object']['fields'] : '',
    '#multiple' => TRUE,
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Outputs warning page.
 */
function shredder_step_5($form, &$form_state) {
  $form['warning'] = array(
    '#markup' => '<div class="messages error">' . t('Are you sure you want to delete all data from selected fields?') . '</div>',
  );
  return $form;
}

/**
 * Runs butch process.
 */
function shredder_wizard_finish(&$form_state) {
  module_load_include('inc', 'shredder', 'shredder.batch');
  $entities = shredder_get_entities_to_change($form_state);
  $operations = array();
  foreach ($entities as $entity_type => $entity_ids) {
    $operations[] = array('shredder_shredder_values', array($entity_type, $entity_ids));
  }
  $batch = array(
    'operations' => $operations,
    'progress_message' => 'Completed @current of @total entity types.',
    'finished' => 'shredder_batch_finished',
    'file' => drupal_get_path('module', 'shredder') . '/shredder.batch.inc',
  );
  batch_set($batch);
  batch_process('admin/config/content/shredder');
}

/**
 * Truncate shredder_changed_entities table.
 */
function shredder_clear_results_table($form, &$form_state) {
  db_truncate('shredder_changed_entities')->execute();
  drupal_set_message('Results table successfully cleared.');
  $form_state['redirect'] = array('admin/config/content/shredder');
}

/**
 * Returns entity types.
 */
function shredder_get_entity_types() {
  $entity_info = entity_get_info();
  $entity_types = array();
  foreach ($entity_info as $entity_name => $entity) {
    if (!empty($entity['bundles'])) {
      $entity_types[$entity_name] = $entity['label'];
    }
  }
  return $entity_types;
}

/**
 * Returns bundles.
 */
function shredder_get_bundles($form_state) {
  $options = array();
  if (!empty($form_state['object']['entity_type'])) {
    $field_info_bundles = field_info_bundles();
    foreach ($form_state['object']['entity_type'] as $entity_type) {
      foreach ($field_info_bundles[$entity_type] as $bundle_name => $bundle) {
        $options[$bundle_name] = $bundle['label'];
      }
    }
  }
  return $options;
}

/**
 * Returns field's types.
 */
function shredder_get_field_types() {
  $supported_field_types = array('text', 'text_long', 'text_with_summary');
  $field_types = field_info_field_types();
  $options = array();
  foreach ($supported_field_types as $field_type) {
    $options[$field_type] = $field_types[$field_type]['label'];
  }
  $options['entity_property'] = t('Entity property');
  return $options;
}

/**
* Returns fields.
*/
function shredder_get_fields($form_state) {
  $supported_entity_properties = array(
    'node' => array('title' => t('Node title')),
    'user' => array('name' => t('User name'), 'mail' => t('User email')),
    'file' => array('filename' => t('File name')),
    'comment' => array('subject' => t('Comment subject')),
  );

  $options = array();
  if (!empty($form_state['object']['entity_type'])) {

    // Add fields.
    foreach ($form_state['object']['bundles'] as $bundle) {
      $fields = field_read_fields(array('bundle' => $bundle));
      foreach ($fields as $field_name => $field) {
        if (in_array($field['type'], $form_state['object']['field_type'])) {
          $options[$field_name] = $field['field_name'];
        }
      }
    }
    // Add entity properties.
    if (in_array('entity_property', $form_state['object']['field_type'])) {
      foreach ($supported_entity_properties as $entity_type => $properties) {
        if (in_array($entity_type, $form_state['object']['entity_type'])) {
          foreach ($properties as $property => $property_title) {
            $options[$property] = $property_title;
          }
        }
      }
    }
  }
  return $options;
}



/**
 * Cache helpers.
 */
function shredder_cache_set($id, $object) {
  ctools_include('object-cache');
  ctools_object_cache_set('shredder', $id, $object);
}

/**
 * Get the current object from the cache, or default.
 */
function shredder_cache_get($id) {
  ctools_include('object-cache');
  return ctools_object_cache_get('shredder', $id);
}

/**
 * Clear the wizard cache.
 */
function shredder_cache_clear($id) {
  ctools_include('object-cache');
  ctools_object_cache_clear('shredder', $id);
}

/**
 * Callback for continue button.
 */
function shredder_wizard_next(&$form_state) {
  if (empty($form_state['object'])) {
    $form_state['object'] = $form_state['values'];
  }
  $form_state['object'] = $form_state['values'] + $form_state['object'];

  shredder_cache_set($form_state['object_id'], $form_state['object']);
}
