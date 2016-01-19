<?php

/**
 * @file
 * Functions for batch importation of Ooyala videos from Backlot.
 */

/**
 * Menu callback to list all videos from Backlot for importing as nodes.
 */
function ooyala_import_multiple($form, $form_state) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  $videos = ooyala_api_video_query();

  // If doing a confirmation form, short cut the rest of the process.
  if (isset($form_state['storage']['import'])) {
    $form = ooyala_import_confirm($form, $form_state);
    $form['video_info'] = array();
    $list = array();
    foreach ($form_state['storage']['import'] as $embed_code) {
      $list[] = check_plain($videos[$embed_code]['name']);
    }
    $form['list'] = array(
      '#markup' => theme('item_list', array('items' => $list)),
    );
    $form = confirm_form($form, t('Are you sure you want to import these videos?'), NULL, '', t('Import'));
    return $form;
  }

  $type = variable_get('ooyala_primary_content_type', FALSE);
  $form = array();
  if (!empty($videos)) {
    // Fetch the statuses for all videos.
    $statuses = ooyala_get_status($type, array_keys($videos));
    foreach ($videos as $embed_code => $data) {
      // Add the video to the video info table.
      $videos[$embed_code] = array(
        check_plain($data['name']),
        l('Import', 'admin/content/ooyala/import/' .$embed_code),
      );
      $form['video_info'][$embed_code]['title'] = array('#markup' => check_plain($data['name']));
      $form['video_info'][$embed_code]['description'] = array('#markup' => check_markup($data['description'], filter_default_format()));
      foreach (array('status') as $meta) {
        // TODO: PORT: Removing 'width', 'height', from the list, being that it
        // doesn't seem to be include in the matadata anymore. Even if I do a direct
        // metadata query. Eric J. Duran <eric.duran@lullbot.com>
        $form['video_info'][$embed_code]['metadata'][$meta] = array('#markup' => check_plain($data[$meta]));
      }

      // Check if this embed_code has already been imported.
      $status = !empty($statuses[$embed_code]);
      $form['video_info'][$embed_code]['status'] = array('#value' => $status ? $status : t('Unimported'));

      // Only show the "Import" link if not yet imported.
      if ($status == FALSE) {
        $form['video_info'][$embed_code]['operations'] = array('#markup' => l('Import', 'admin/content/ooyala/import/'. $embed_code));
      }
      else {
        $form['video_info'][$embed_code]['operations'] = array();
      }

      // Add the video to the list of checkboxes.
      $options[$embed_code] = check_plain($data['name']);
    }

    $header = array(t('Video information'), t('Operations'));

    $form['custom']['table'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $videos,
    );

    $form['buttons']['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('Import selected'),
      '#access' => variable_get('ooyala_primary_content_type', FALSE),
    );
  }
  return $form;
}

/**
 * Submit handler ooyala_import().
 */
function ooyala_import_multiple_submit($form, &$form_state) {
  // If it hasn't been confirmed yet, reload the form with the confirmation.
  if (!isset($form_state['storage']['import'])) {
    $form_state['storage']['import'] = array_intersect_key($form_state['values']['table'], array_filter($form_state['values']['table']));
    $form_state['rebuild'] = TRUE;
    return;
  }

  // After the confirmation, start the Batch API process to import the videos.
  $videos = $form_state['storage']['import'];
  $type = $form_state['values']['content_type'];

  unset($form_state['storage']['import']);

  // Batch it up.
  $batch = array(
    'operations' => array(
      array('ooyala_import_batch_process', array($videos, $type)),
    ),
    'finished' => 'ooyala_import_batch_finished',
    'title' => t('Importing videos from Ooyala Backlot'),
    'init_message' => t('Import starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Import has encountered an error.'),
    'file' => drupal_get_path('module', 'ooyala') . '/includes/ooyala.import.inc',
  );
  batch_set($batch);
}

/**
 * Theme the ooyala_import_multiple() form.
 */
function theme_ooyala_import_multiple($vars) {
  $form = $vars['form'];
  // Build up a list of all the videos in a table.
  $rows = array();
  foreach (element_children($form['video_info']) as $key) {
    // Fetch info.
    $title = drupal_render($form['video_info'][$key]['title']);
    // Localize theme description.
    $description = drupal_render($form['video_info'][$key]['description']);
    // Meta data.
    $data = array();
    foreach (element_children($form['video_info'][$key]['metadata']) as $meta) {
      $data[] = '<strong>'. $meta .'</strong>: '. drupal_render($form['video_info'][$key]['metadata'][$meta]);
    }
    $description .= '<div class="metadata">'. implode(', ', $data) .'</div>';
    $ops = drupal_render($form['video_info'][$key]['operations']);

    if (isset($title)) {
      // Style video info.
      $video = '';
      $video .= '<div class="ooyala-video-info">';
      $video .= '<strong><label for="edit-import-'. $key .'">'. $title .'</label></strong>';
      $video .= '<div class="description">'. $description .'</div>';
      $video .= '</div>';
    }

    // Build rows.
    $row = array();
    $rows[$key] = array($video, $ops);
  }

  $form['custom']['table']['#options'] = $rows;
  $output = '';
  if ($rows) {
    drupal_add_css(drupal_get_path('module', 'ooyala') . '/ooyala.css');
  }
  elseif (isset($form['video_info'])) {
    $output .= '<p>' . t('There are no videos in your Ooyala Backlot account, or Ooyala module was unable to reach the Ooyala server.') . '</p>';
  }

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Menu callback for importing a single item from Backlot.
 */
function ooyala_import_confirm($form, $form_state, $item = NULL) {
  // Allow the user to determine what type of node this should be.
  $options = ooyala_type_names();
  $default = variable_get('ooyala_primary_content_type', NULL);
  $form['content_type'] = array(
    '#type' => 'select', 
    '#title' => t('Import as'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => isset($options[$default]) ? $default : NULL,
    '#description' => t('This type of content will be used for the import.'),
  );

  if (empty($options)) {
    $form['content_type']['#disabled'] = TRUE;
    $form['content_type']['#options'][''] = t('No available types');
    drupal_set_message(t('There are no content types that have an Ooyala field configured. At least one Ooyala field must be added to your site before content can be imported.'), 'warning');
  }

  // If doing a batch-import, the selection form is all we need.
  if (!isset($item)) {
    return $form;
  }

  // Video information.
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video info'),
    '#collapsable' => FALSE,
    '#weight' => 20,
  );

  foreach ($item as $key => $val) {
    $item_desc[] = '<strong>' . check_plain($key) . '</strong>: '. (is_array($val) ? check_plain(implode(', ', $val)) : check_plain($val));
  }
  $form['info']['item_desc'] = array(
    '#markup' => theme('item_list', array('items' => $item_desc)),
  );
  $form['item'] = array(
    '#type' => 'value', 
    '#value' => $item,
  );

  return confirm_form($form, t('Are you sure you want to import this video?'), 'admin/content/ooyala/import', '', t('Import'));
}

/**
 * Submit function for ooyala_import_confirm().
 */
function ooyala_import_confirm_submit($form, &$form_state) {
  $item = $form_state['values']['item'];
  $type = $form_state['values']['content_type'];

  // Fix the content type name
  $item['content_type'] = $type;
  // Create a node out of it.
  $node = ooyala_create_node($item, $type);

  // Pull in the thumbnail for the new node.
  ooyala_api_fetch_image($item['embed_code']);

  // Redirect to the node.
  $form_state['redirect'] = array('node/' . $node->nid);
}

/**
 * Batch processor callback.
 */
function ooyala_import_batch_process($videos, $type, &$context) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($videos);
    $context['sandbox']['videos'] = $videos;
  }

  // Process videos by groups of 20.
  $count = min(20, count($context['sandbox']['videos']));
  for ($i = 1; $i <= $count; $i++) {
    // For each nid, load the node, reset the values, and save it.
    $embed_code = array_shift($context['sandbox']['videos']);
    // Since we only have the embed code, load the rest of the metadata.
    $video_data = ooyala_api_video_load($embed_code);
    $video = array(
      'name' => $video_data['name'],
      'embed_code' => $embed_code,
      'status'  => $video_data['status'],
      'duration' => ($video_data['duration']) ? $video_data['duration'] : 0,
    );
    $node = ooyala_create_node($video, $type);
    // Pull in the thumbnail for the new node.
    ooyala_api_fetch_image($video['embed_code']);

    // Store result for post-processing in the finished callback.
    $context['results'][] = l($node->title, 'node/' . $node->nid);

    // Update our progress information.
    $context['sandbox']['progress']++;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch 'finished' callback.
 */
function ooyala_import_batch_finished($success, $results, $operations) {
  if (!$success) {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
  }
  $message = format_plural(count($results), '1 item successfully processed:', '@count items successfully processed:');
  $message .= theme('item_list', array($results));
  drupal_set_message($message);
}

/**
 * Check to see if this video has already been imported.
 *
 * @params $type_name
 *   The machine name of the content type.
 * @param $keys
 *   An array of video keys to fetch the imported status for.
 *
 * @return
 *   An array of node IDs, keyed by the embed code of the video associated with
 *   the node.
 */
function ooyala_get_status($type_name, $keys = array()) {
  if (empty($keys) || empty($type_name)) {
    return;
  }

  $return = array();
  $field = ooyala_field_names($type_name);
  if (empty($field)) {
    return;
  }
  $field_name = 'field_data_' .$field[0];
  $column_name = $field[0] .'_value';
  
  $query = db_select($field_name, 'f')
    ->fields('f')
    ->condition('f.' .$column_name, $keys, 'IN');
  $result = $query->execute()->fetchAll();
  foreach ($result as $index => $row) {
    $return[$row->$column_name] = $row->entity_id;
  }

  return $return;
}
