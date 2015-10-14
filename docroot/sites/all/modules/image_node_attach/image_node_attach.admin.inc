<?php

/**
 * @file
 * Admin settings for image node attach module.
 */

/**
 * Image Node Attach admin form.
 *
 * Choose content type where image attach needs to be migrate.
 */
function image_node_attach_admin_form($form, &$form_state) {
  $form = array();
  $form['image_node_attach_content_type_names'] = array(
    '#type' => 'select',
    '#title' => t('Content Type'),
    '#options' => node_type_get_names(),
    '#description' => t('Select the content type where image attach field already exist.'),
    '#required' => TRUE,
    '#multiple' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  // If image_attach not exist, prevent submission.
  // Find out if we have a image_attach table in database, if no disabled the
  // submit button.
  if (!db_table_exists('image_attach')) {
    drupal_set_message(t("You don't currently have any image attach nodes. As a result, this form is disabled."), 'warning');
    $form['submit']['#disabled'] = TRUE;
  }

  return $form;
}

/**
 * Form Submit handler.
 *
 * Create the entity reference fields for every selected content type and set
 * batch callback function.
 */
function image_node_attach_admin_form_submit($form, &$form_state) {
  $content_type_names = $form_state['values']['image_node_attach_content_type_names'];
  // Create entity reference field before migration start.
  image_node_attach_batch_entity_reference($content_type_names);
  // Create batch callback function.
  $function = 'image_node_attach_batch_content_type';
  $batch = $function($content_type_names);
  batch_set($batch);
}

/**
 * Batch : Prepare a batch definition.
 *
 * This creates an operations array defining what batch should do for every
 * node, including what it should do when it's finished.
 *
 * @param string $content_type_names
 *   Content type names selected from admin page.
 */
function image_node_attach_batch_content_type($content_type_names = array()) {
  $node_count = db_query("SELECT DISTINCT node.nid FROM {image_attach} INNER JOIN {node} ON node.nid = image_attach.nid WHERE node.type IN(:node_type)", array(":node_type" => $content_type_names))->rowCount();
  $operations = array();
  // Set up an operations array.
  // Each operation in the operations array means at least one new HTTP request,
  // running Drupal from scratch to accomplish the operation. If the operation
  // returns with $context['finished'] != TRUE, then it will be called again.
  // This operation would attach image nodes with respective content type
  // entity reference field.
  $operations[] = array(
    'image_node_attach_batch_image_attach', array($content_type_names, $node_count),
  );

  $batch = array(
    'operations' => $operations,
    'finished' => 'image_node_attach_batch_finished',
    'file' => drupal_get_path('module', 'image_node_attach') . '/image_node_attach.admin.inc',
    'title' => t('Attaching images nodes'),
    'progress_message' => t('Processed @current out of @total.'),
    'init_message' => t('Migration is starting.'),
    'error_message' => t('Migration has encountered an error.'),
  );
  return $batch;
}

/**
 * Function to create field for selected content types.
 *
 * This is the function that create entity reference field with image formatter
 * in selected content types.
 *
 * @param string $content_type_names
 *   Content type name.
 */
function image_node_attach_batch_entity_reference($content_type_names = array()) {
  $field_name = 'field_image_node_attach';
  if (!empty($content_type_names)) {
    foreach ($content_type_names as $bundle) {
      // Make sure the field doesn't already exist.
      if (!field_info_field($field_name)) {
        // Create the field.
        $field = array(
          'field_name' => $field_name,
          'type' => 'entityreference',
          'cardinality' => -1,
          'settings' => array(
            'target_type' => 'node',
            'handler' => 'base',
            'handler_settings' => array(
              'target_bundles' => array('image'),
            ),
          ),
        );
        field_create_field($field);
      }
      if (!field_info_instance('node', $field_name, $bundle)) {
        // Create the instance.
        $instance = array(
          'field_name' => $field_name,
          'entity_type' => 'node',
          'bundle' => $bundle,
          'label' => 'Image Attached',
          'description' => 'Image nodes attached as nodes.',
          'required' => TRUE,
          'widget' => array(
            'active' => 1,
            'module' => 'options',
            'settings' => array(
              'content_taxonomy_opt_groups' => FALSE,
            ),
            'type' => 'options_select',
            'weight' => 33,
          ),
          'display' => array(
            'default' => array(
              'label' => 'above',
              'module' => 'image_node_attach',
              'settings' => array(
                'image_link' => '',
                'image_style' => '',
              ),
              'type' => 'image_node_attach_entity_image_formatter',
              'weight' => 3,
            ),
            'teaser' => array(
              'label' => 'above',
              'settings' => array(),
              'type' => 'hidden',
              'weight' => 0,
            ),
          ),
          'formatter' => array(
            'label'  => t('label'),
            'format' => 'image_node_attach_entity_image_formatter',
          ),
        );
        field_create_instance($instance);
      }
    }
  }
}

/**
 * Batch operation for selected content types.
 *
 * This is the function that would associate image nodes with respective content
 * type entity reference field.
 *
 * @param string $content_type_names
 *   Content type name.
 * @param int $node_count
 *   Total nodes count need to migrate.
 */
function image_node_attach_batch_image_attach($content_type_names, $node_count, &$context) {
  if (!empty($content_type_names)) {
    if (empty($context['sandbox'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['current_node'] = 0;
      $context['sandbox']['max'] = $node_count;
    }
    // Initialize limit that we can safely process at a time without a timeout.
    $limit = 50;
    $image_attach_result = db_query_range("SELECT DISTINCT node.nid FROM {image_attach} INNER JOIN {node} ON node.nid = image_attach.nid WHERE node.type IN(:node_type) AND node.nid > :current_node ORDER BY node.nid", 0, $limit, array(":node_type" => $content_type_names, ":current_node" => $context['sandbox']['current_node']));
    if ($image_attach_result->rowCount() > 0) {
      foreach ($image_attach_result as $image_attach_data) {
        $node = image_node_attach_node_attachment($image_attach_data->nid);
        // Store some results for post-processing in the 'finished' callback.
        // The contents of 'results' will be available as $results in the
        // 'finished' function.
        $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
        $context['sandbox']['progress']++;
        $context['sandbox']['current_node'] = $node->nid;
        // Optional message displayed under the progressbar.
        $context['message'] = t('Migrating node "@title"', array('@title' => $node->title));
      }
    }
    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
  }
}

/**
 * Gather node array and attached image nodes.
 *
 * @param int $nid
 *   Node nid where image nodes would attached.
 *
 * @return node array.
 *   return node array object.
 */
function image_node_attach_node_attachment($nid = '') {
  $node = node_load($nid);
  $image_attach_iids = db_query("SELECT iid FROM {image_attach} WHERE nid = :nid", array(":nid" => $nid));
  if ($image_attach_iids->rowCount() > 0) {
    foreach ($image_attach_iids as $image_attach_iid) {
      $node->field_image_node_attach[LANGUAGE_NONE][]['target_id'] = $image_attach_iid->iid;
      // Delete this row entry from image_attach table.
      db_delete('image_attach')
              ->condition('nid', $nid)
              ->condition('iid', $image_attach_iid->iid)
              ->execute();
    }
  }
  // Update node with attached images.
  node_save($node);
  return $node;
}

/**
 * Batch 'finished' callback.
 */
function image_node_attach_batch_finished($success, $results) {
  if ($success) {
    // We just display the number of nodes we processed...
    drupal_set_message(t('@count results processed.', array('@count' => count($results))));
    if (count($results) > 0) {
      drupal_set_message(t('The final result was "%final"', array('%final' => end($results))));
    }
  }
  else {
    // An error occurred.
    drupal_set_message(t('An error occurred. Check the error log of site. See <a href="@dblog">Recent log messages</a>', array('@dblog' => module_exists('dblog') ? url('admin/reports/dblog') : '')), 'error');
  }
}
