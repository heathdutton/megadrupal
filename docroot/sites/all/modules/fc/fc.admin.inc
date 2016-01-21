<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - admin.
 */

function fc_settings($form, &$form_state) {

  $form['fc_tooltip'] = array(
    '#type' => 'textfield',
    '#title' => t('Tooltip'),
    '#description' => t('This is the tooltip message that appears when hovering over the field complete marker.'),
    '#default_value' => variable_get('fc_tooltip', 'This field can be left blank but adds to the completeness of this item.'),
  );

  $form['fc_js_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add field IDs to page'),
    '#description' => t('If checked the Field Complete module adds any form fields that are set for field completeness to Drupal settings (Drupal.settings.ec.ids).'),
    '#default_value' => variable_get('fc_js_ids', FALSE),
  );

  return system_settings_form($form);
}

function fc_rebuild($form, &$form_state) {
  $form['msg'] = array(
    '#type' => 'item',
    '#title' => t('Rebuild the Field Complete table'),
    '#description' => t('This action cannot be reversed and should not be interrupted. It may take a long time for big sites.'),
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Rebuild'),
    ),
  );
  return $form;
}

function fc_rebuild_submit($form, &$form_state) {
  // The point of no return: Wipe the table completely
  db_delete('fc')->execute();

  // Build the batch data
  $batch = array(
    'operations' => array(),
    'finished' => 'fc_rebuild_finished',
    'title' => t('Rebuilding Field Complete table'),
    'init_message' => t('Rebuild of Field Complete table is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Rebuild of Field Complete table has encountered an error.'),
    'file' => drupal_get_path('module', 'fc') . '/fc.admin.inc',
  );

  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (empty($entity_info['uri callback']) || empty($entity_info['fieldable'])) {
      // There's no display URI for this entity so it'll never appear
      // rendered, or it's not fieldable - so Field Complete doesn't apply
      continue;
    }
    $batch['operations'][] = array('fc_rebuild_process', array($entity_type, $entity_info));
  }

  batch_set($batch);
}

/**
 * Rebuild the 'fc' table to incorporate all existing entities
 */
function fc_rebuild_process($entity_type, $entity_info, &$context) {
  $idKey = $entity_info['entity keys']['id'];
  $baseTable = $entity_info['base table'];

  if (empty($context['sandbox']['initialised'])) {
    $context['sandbox']['initialised'] = TRUE;

    $context['sandbox']['current_id'] = 0;
    $context['sandbox']['progress'] = 0;

    // We need to ensure there's no 'zero' ID - true for 'users'
    $context['sandbox']['max'] = db_select($baseTable, 'e')
      ->condition($idKey, $context['sandbox']['current_id'], '>')
      ->countQuery()->execute()->fetchField();

    if ($context['sandbox']['max'] == 0) {
      return;
    }
  }

  $ids = db_select($baseTable, 'e')->fields('e', array($idKey))
    ->condition($idKey, $context['sandbox']['current_id'], '>')
    ->range(0, 10)->orderBy("e.$idKey", 'ASC')->execute()->fetchCol();

  if (!empty($ids)) {
    $args = array(
    '%entity_type' => $entity_type,
    '%idKey' => $idKey,
    '%id' => NULL,
    );
    foreach (entity_load($entity_type, $ids) as $entity) {
      $entity->fc = fcComplete::build($entity_type, $entity);
      $entity->fc->completeness();
      $entity->fc->save(fcComplete::SKIP_IF_EXISTS);

      $context['sandbox']['current_id'] = $args['%id'] = $entity->$idKey;
      $context['sandbox']['progress']++;
    }
    $context['message'] = t('Now processing %entity_type with %idKey=%id', $args);
  }

  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['results'][] = t('Processed %count entities for %entity_type', array('%count' => $context['sandbox']['max'], '%entity_type' => $entity_type));
    $context['finished'] = TRUE;
  }
}

/**
 * Batch 'finished' callback
 */
function fc_rebuild_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
}
