<?php

/**
 * @file
 * Contains admin functions for field_library module.
 */

/**
 * Form builder.
 */
function field_library_add_confirm($form, $form_state, $entity_type, $bundle, $field_library_bundle) {
  $path = _field_ui_bundle_admin_path($entity_type, $bundle);
  $entity_info = entity_get_info($entity_type);
  if (empty($entity_info) || empty($entity_info['bundles'][$bundle])) {
    drupal_not_found();
  }
  $fields = field_info_instances('field_library', $field_library_bundle->name);

  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );
  $form['field_library_bundle'] = array(
    '#type' => 'value',
    '#value' => $field_library_bundle,
  );
  $list = array();
  foreach ($fields as $instance) {
    $list[] = check_plain($instance['label']);
  }
  $form['fields'] = array(
    '#type' => 'markup',
    '#markup' => theme('item_list', array('items' => $list, 'title' => t('Fields to be added'))),
  );
  $form = confirm_form($form, 'Are you sure?', $path, t('You are about to add the following fields to the %bundle bundle of the %entity_type entity type.', array(
    '%bundle' => $entity_info['bundles'][$bundle]['label'],
    '%entity_type' => $entity_info['label'],
  )), t('Continue'), t('Cancel'));
  $form['description']['#weight'] = -1;
  return $form;
}

/**
 * Submission handler.
 */
function field_library_add_confirm_submit($form, $form_state) {
  $values = $form_state['values'];
  $bundle = $values['bundle'];
  $entity_type = $values['entity_type'];
  $field_library_bundle = $values['field_library_bundle'];
  $fields = field_info_instances('field_library', $field_library_bundle->name);
  $batch = array(
    'title' => t('Adding fields from %library bundle', array(
      '%library' => $field_library_bundle->name,
    )),
    'finished' => 'field_library_add_finished',
    'file' => drupal_get_path('module', 'field_library') . '/field_library.admin.inc',
    'operations' => array(),
  );
  foreach ($fields as $field) {
    $batch['operations'][] = array(
      'field_library_add_batch',
      array(
        $entity_type,
        $bundle,
        $field['field_name'],
        $field_library_bundle->name,
      ),
    );
  }
  batch_set($batch);
}

/**
 * Batch worker callback.
 */
function field_library_add_batch($entity_type, $bundle, $field_name, $field_library_bundle, &$context) {
  $instance = field_info_instance($entity_type, $field_name, $bundle);
  if (!$instance) {
    $existing_instances = count(field_info_instances($entity_type, $bundle));
    $entity_info = entity_get_info($entity_type);
    $instance = field_info_instance('field_library', $field_name, $field_library_bundle);
    $instance['entity_type'] = $entity_type;
    $instance['bundle'] = $bundle;
    // Update weights to avoid display collisions.
    foreach ($instance['display'] as $view_mode => $settings) {
      $weight = $existing_instances + 1;
      if (!empty($context['results'])) {
        $weight += count($context['results']);
      }
      $instance['display'][$view_mode]['weight'] = $weight;
    }
    field_create_instance($instance);
    $message = t('Added the %field_name field to the %bundle bundle of the %entity_type entity type.', array(
      '%bundle' => $entity_info['bundles'][$bundle]['label'],
      '%entity_type' => $entity_info['label'],
      '%field_name' => $instance['label'],
    ));
    $context['results'][] = array(
      'message' => $message,
      'entity_type' => $entity_type,
      'bundle' => $bundle,
    );
    $context['message'] = $message;
  }
}

/**
 * Batch finished callback.
 */
function field_library_add_finished($success, $results, &$operations) {
  $redirect = '';
  if ($success) {
    foreach ($results as $result) {
      drupal_set_message($result['message']);
      if (!$redirect) {
        $redirect = _field_ui_bundle_admin_path($result['entity_type'], $result['bundle']) . '/fields';
      }
    }
    field_info_cache_clear();
  }
  else {
    drupal_set_message(t('An error occurred trying to copy the fields from the library to your entity bundle.'));
  }
  if (!empty($redirect)) {
    drupal_goto($redirect);
  }
}
