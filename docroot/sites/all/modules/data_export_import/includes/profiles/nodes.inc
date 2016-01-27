<?php
/**
 * @file
 * Enables nodes to be exported and imported.
 */

/**
 * Callback function to export nodes.
 */
function data_export_import_callback_export_nodes() {

  return drupal_get_form('data_export_import_export_nodes_form');
}

/**
 * Function to create form to export nodes.
 */
function data_export_import_export_nodes_form($form_state) {

  $form['export_nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export nodes'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['export_nodes']['description'] = array(
    '#type' => 'item',
    '#title' => t('Export all nodes to a dataset file'),
  );

  node_types_rebuild();
  $node_types = node_type_get_types();

  foreach ($node_types as $node_type) {

    $node_type_type = $node_type->type;
    $node_type_name = $node_type->name;

    $form['export_nodes']['content_types'][$node_type_type] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($node_type_name),);
  }

  // Adds a simple submit button that refreshes the form and clears its
  // contents. This is the default behavior for forms.
  $form['export_nodes']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create dataset file'),
  );

  return $form;
}

/**
 * Function to process form to export nodes.
 */
function data_export_import_export_nodes_form_submit($form, &$form_state) {

  // This module has not yet been extended to correctly handle data
  // which has internationalization (i18n) enabled.
  if (module_exists('i18n')) {

    drupal_set_message(t("The data export functionality is not currently compatible with internationalization (i18n)."), 'error');
    return TRUE;
  }

  data_export_import_export_nodes_to_file($form_state);

  return TRUE;
}

/**
 * Export the required dataset files.
 *
 * This function will look at which content types have been selected
 * for exporting to file and call a function to export thoese content
 * types.  The $dataset_files_created variable will hold the names of
 * all the dataset files which were created so it can be handed back
 * to the calling code for display to the user.
 *
 * @param array $form_state
 *   Current values held in the form.
 *
 * @return bool
 *   TRUE if all ran OK.
 */
function data_export_import_export_nodes_to_file($form_state) {

  // See if any content types were selected - if none then exit out
  // gracefully.
  $at_least_one_content_type_is_selected_flag = FALSE;
  foreach ($form_state['values']['export_nodes']['content_types'] as $content_type => $value) {
    if ($value == 1) {

      $at_least_one_content_type_is_selected_flag = TRUE;
    }
  }

  if (!$at_least_one_content_type_is_selected_flag) {
    drupal_set_message(t("No content types selected."));
    return TRUE;
  }

  // Create the default directory to hold the datasets.
  $dataset_directory_parent_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import";
  file_prepare_directory($dataset_directory_parent_directory, FILE_CREATE_DIRECTORY);

  $dataset_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/nodes/";
  file_prepare_directory($dataset_directory, FILE_CREATE_DIRECTORY);

  // Adding in the main values to the $batch variable.
  $batch = array();
  $batch['finished'] = 'data_export_import_batch_export_nodes_finished';
  $batch['title'] = t('Exporting nodes');
  $batch['init_message'] = t('The exportation of nodes starting.');
  $batch['progress_message'] = t('Processed @current out of @total.');
  $batch['error_message'] = t('Exporting nodes has encountered an error.');
  $batch['file'] = drupal_get_path('module', 'data_export_import') . '/includes/profiles/nodes.inc';

  // We can loop through which content types need to be exported.
  // The #post array will only contain values which have been set to
  // have a value of '1'. We will build up the $batch object with the
  // required operations.
  foreach ($form_state['values']['export_nodes']['content_types'] as $content_type => $value) {

    // This should mean that the rest of this loop will not be called if the
    // content_type was not selected.
    if ($value == 0) {
      continue;
    }

    $file_name = format_date(REQUEST_TIME, 'custom', 'Ymd_His') . "_nodes_" . $content_type . ".dataset";
    $file_path_and_name = $dataset_directory . "/" . $file_name;

    // Rebuild is needed to flush out content types which may have been deleted.
    node_types_rebuild();
    $node_types = node_type_get_types();

    // Save the content type variable to the file. By serializing the
    // variable we will change it to a character based format which is
    // safe to be output to a file.  This is made safer then by being
    // base64 encoded to make sure line endings and other characters
    // do not cause issues on importing the dataset.
    $content_type_data_serialized = serialize($node_types[$content_type]);
    $content_type_data_serialized_and_encoded = base64_encode($content_type_data_serialized);
    file_unmanaged_save_data($content_type_data_serialized_and_encoded . "\n", $file_path_and_name, FILE_EXISTS_REPLACE);

    // Here we will loop through the fields in the content type and
    // look for fields which are of type 'file'.  We will then send an
    // array of these fields into the
    // data_export_import_batch_export_nodes_to_file function - this
    // will mean that the
    // data_export_import_batch_export_nodes_to_file function will
    // know which are the file fields so it can store the file data.
    $fields_of_type_file = array();
    $fields_info = field_info_instances('node', $content_type);

    foreach ($fields_info as $field_name => $field_value) {

      $field_info = field_info_field($field_name);
      $type = $field_info['type'];

      if ($type == 'file' || $type == 'image') {

        $fields_of_type_file[] = $field_name;
      }
    }

    // Each content type being exported to a dataset file will be run
    // as a batch to prevent timeouts.
    $batch['operations'][] = array(
      'data_export_import_batch_export_nodes_to_file',
      array(
        $content_type,
        $file_path_and_name,
        $fields_of_type_file,
      ),
    );
  }

  // This is the key function which will set the batch up to be processed.
  batch_set($batch);

  // Since we are not calling this batch processing from a form we will need to
  // request that the batch is processed.
  batch_process('admin/config/system/data_export_import/nodes');

  return TRUE;
}

/**
 * Export the required dataset files via drush.
 *
 * This function is set up to specifically be called by drush.
 *
 * This function will look at which content types have been selected
 * for exporting to file and call a function to export thoese content
 * types.  The $dataset_files_created variable will hold the names of
 * all the dataset files which were created so it can be handed back
 * to the calling code for display to the user.
 *
 * @param array $content_types
 *   Current values held in the form.
 *
 * @return bool
 *   TRUE if all ran OK.
 */
function data_export_import_export_nodes_to_file_using_drush($content_types) {

  // See if any content types were selected - if none then exit out
  // gracefully.
  if (!isset($content_types)) {

    return TRUE;
  }

  // Create the default directory to hold the datasets.
  $dataset_directory_parent_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import";
  file_prepare_directory($dataset_directory_parent_directory, FILE_CREATE_DIRECTORY);

  $dataset_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/nodes/";
  file_prepare_directory($dataset_directory, FILE_CREATE_DIRECTORY);

  // Adding in the main values to the $batch variable.
  $batch = array();
  $batch['error_message'] = t('Exporting nodes has encountered an error.');

  $batch['file'] = drupal_get_path('module', 'data_export_import') . '/includes/profiles/nodes.inc';

  // We can loop through which content types need to be exported.
  // The #post array will only contain values which have been set to
  // have a value of '1'. We will build up the $batch object with the
  // required operations.
  $files_created = array();
  foreach ($content_types as $content_type) {

    // Get the filename we are going to save the data to.
    $file_name = format_date(REQUEST_TIME, 'custom', 'Ymd_His') . "_nodes_" . $content_type . ".dataset";
    $file_path_and_name = $dataset_directory . "/" . $file_name;

    // Save the filename so it can be displayed when the command has
    // been called.
    $files_created[] = $file_name;

    // Here we are getting all content types and selecting just the one
    // we need - this seems to give more consistent results than just
    // extracting the single node object.
    node_types_rebuild();
    $node_types = node_type_get_types();

    // Save the content type variable to the file. By serializing the
    // variable we will change it to a character based format which is
    // safe to be output to a file.  This is made safer then by being
    // base64 encoded to make sure line endings and other characters
    // do not cause issues on importing the dataset.
    $content_type_data_serialized = serialize($node_types[$content_type]);

    $content_type_data_serialized_and_encoded = base64_encode($content_type_data_serialized);
    file_unmanaged_save_data($content_type_data_serialized_and_encoded . "\n", $file_path_and_name, FILE_EXISTS_REPLACE);

    // Here we will loop through the fields in the content type and
    // look for fields which are of type 'file'.  We will then send an
    // array of these fields into the
    // data_export_import_batch_export_nodes_to_file function - this
    // will mean that the
    // data_export_import_batch_export_nodes_to_file function will
    // know which are the file fields so it can store the file data.
    $fields_of_type_file = array();
    $fields_info = field_info_instances('node', $content_type);
    foreach ($fields_info as $field_name => $field_value) {
      $field_info = field_info_field($field_name);
      $type = $field_info['type'];

      if ($type == 'file' || $type == 'image') {

        $fields_of_type_file[] = $field_name;

      }
    }

    // Each content type being exported to a dataset file will be run
    // as a batch to prevent timeouts.
    $batch['operations'][] = array(
      'data_export_import_batch_export_nodes_to_file',
      array(
        $content_type,
        $file_path_and_name,
        $fields_of_type_file,
      ),
    );
  }

  // This is the key function which will set the batch up to be
  // processed.
  $batch['progressive'] = FALSE;
  batch_set($batch);

  // Since we are not calling this batch processing from a form we
  // will need to request that the batch is processed.
  drush_backend_batch_process();

  return implode("\n", $files_created);
}

/**
 * Batch function called to export the content type.
 *
 * @param string $content_type
 *   The dataset file which is being imported.
 *
 * @param string $file_path_and_name
 *   The path and filename to use for the dataset file created.
 *
 * @return bool
 *   TRUE if all ran OK.
 */
function data_export_import_batch_export_nodes_to_file($content_type, $file_path_and_name, $fields_of_type_file, &$context) {

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = db_query("SELECT COUNT(DISTINCT nid) FROM {node} WHERE type = :content_type", array(':content_type' => $content_type))->fetchField();
  }

  // We will set this to make one pass at a time to limit timeouts.
  $limit = 1;

  $result = db_query_range("SELECT nid FROM {node} WHERE nid > :current_node AND type = :content_type ORDER BY nid ASC", 0, $limit, array(':current_node' => $context['sandbox']['current_node'], ':content_type' => $content_type));

  // The new D7 syntax returns an object not an array.
  foreach ($result as $row) {

    // Output one node to the file.
    $node = node_load($row->nid, NULL, TRUE);

    foreach ($fields_of_type_file as $file_field) {

      $file_field_items = field_get_items('node', $node, $file_field);
      if ($file_field_items) {

        foreach ($file_field_items as $field_item_key => $field_item_value) {

          $handle = fopen($field_item_value['uri'], 'r');
          $file_data = base64_encode(stream_get_contents($handle));
          $node->{$file_field}[LANGUAGE_NONE][$field_item_key]['data_export_import_file_data'] = $file_data;
        }
      }
    }

    // Here we will get the comments for a node and attach them to the node
    // object.
    if (module_exists('comment')) {

      $comments_result = db_query("SELECT cid FROM {comment} WHERE nid = :comment_node ORDER BY cid ASC", array(':comment_node' => $row->nid));
      foreach ($comments_result as $comment_row) {

        $whole_comment = comment_load($comment_row->cid, TRUE);
        $node->comments[] = $whole_comment;
      }
    }

    // Here we will serialize the array to convert it to a string
    // which can then be output to a file.
    $node_serialized = serialize($node);

    // Encode the string to make sure the data does not contain line
    // endings and other characters which may cause problems when
    // reading the file during import.
    $node_serialized_and_base64_encoded = base64_encode($node_serialized);

    file_put_contents($file_path_and_name, $node_serialized_and_base64_encoded . "\n", FILE_APPEND | LOCK_EX);

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $node->nid;
  }

  // Store some result for post-processing in the finished callback.
  // We will use a trick by setting the key to be the file path so
  // this array value will be set again and again.  This way the results
  // array will not get larger and larger for each record which is processed
  // but will hold a single result for this batch.
  $context['results'][$file_path_and_name] = basename($file_path_and_name);

  // Inform the batch engine that we are not finished, and provide an
  // estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch 'finished' callback.
 */
function data_export_import_batch_export_nodes_finished($success, $results, $operations) {
  if ($success) {

    // Here we do something meaningful with the results.
    $message = t('The following dataset files were created:');
    $message .= theme('item_list', array('items' => $results));
  }
  else {

    // An error occurred. $operations contains the operations that
    // remained unprocessed.
    $error_operation = reset($operations);
    $message = t(
      'An error occurred while processing %error_operation with arguments: @arguments',
      array(
        '%error_operation' => $error_operation[0],
        '@arguments' => print_r($error_operation[1], TRUE),
      )
    );
  }

  drupal_set_message($message);
}

/**
 * Callback function to import nodes.
 */
function data_export_import_callback_import_nodes() {

  return drupal_get_form('data_export_import_import_nodes_form');
}

/**
 * Function to create form to import nodes.
 */
function data_export_import_import_nodes_form($form_state) {

  $form = array();

  $form['import_nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import nodes'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Get the contents of the dataset directory and create a list of
  // links to dataset files.
  $directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/nodes";

  $mask = '/.dataset/';
  $files = file_scan_directory($directory, $mask);

  // Sort them by the filename which is used as the key.  Since the
  // files are named using datetime stamps they will be listed in
  // date/time order.
  ksort($files);

  $options = array();
  $options['none'] = t('None');

  foreach ($files as $file) {
    $options[$file->filename] = check_plain($file->filename);
  }

  $form['import_nodes']['dataset_file'] = array(
    '#type' => 'radios',
    '#title' => t('Select file to import.'),
    '#default_value' => 'none',
    '#options' => $options,
  );

  $form['import_nodes']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import dataset files'),
  );

  return $form;
}

/**
 * Function to process form to import nodes.
 */
function data_export_import_import_nodes_form_submit($form, &$form_state) {

  if ($form_state['values']['dataset_file'] != 'none') {

    $result = data_export_import_import_nodes($form_state['values']['dataset_file']);

    if ($result) {
      drupal_set_message(t('The Nodes dataset file %dataset_file was imported.', array('%dataset_file' => $form_state['values']['dataset_file'])));
    }
    else {
      drupal_set_message(t('The Nodes dataset file %dataset_file was not imported.', array('%dataset_file' => $form_state['values']['dataset_file'])), 'error');
    }
  }
}

/**
 * Import a dataset file and make current nodes match exactly.
 *
 * The purpose of this function is to effectively 'import' the nodes
 * stored in a dataset file.
 *
 * NB - When this import has finished the nodes in the receiving
 * instance should be an exact match with the nodes in the imported
 * dataset file.  Think in terms of rsync with the '--delete'
 * option. This means that as well as importing new nodes we need to
 * delete nodes in the receiving instance which are not in the
 * imported dataset.
 *
 * This synchronization will be carried out by two passes.
 *
 * First we will loop through the nodes in the receiving instance  and
 * check against the the imported dataset. If the node exists in the
 * dataset then it will be updated in the receiving instance.  If it
 * doesn't exist in the dataset then it will be deleted from the
 * receiving Drupal instance.
 *
 * The second pass will be a loop through the dataset - any terms
 * which are in the dataset but are not in the receiving Drupal
 * instance will be created.
 *
 * This will effectively mean that the terms have been sychronised
 * completely.
 *
 * NB - There is some deeper logic here we need to be aware of.  The
 * node ID's need to match exactly otherwise the related items will
 * not match.  So when the new nodes are created they will need to
 * have their old ID's set to match exactly - again due to the related
 * terms.
 *
 * @param string $file
 *   The dataset file which is being imported.
 *
 * @return bool
 *   TRUE if the import process ran without errors.
 */
function data_export_import_import_nodes($file) {

  // Read the first line - decode the object and check that the content types
  // are an exact match.
  $file_path = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/nodes/" . $file;

  $handle = fopen($file_path, "r");
  if ($handle) {

    $content_type_line_from_file = fgets($handle);
    fclose($handle);
  }

  $node_content_type_object_from_file = unserialize(base64_decode($content_type_line_from_file));

  // Check that the content types match.
  $node_content_type = $node_content_type_object_from_file->type;

  node_types_rebuild();
  $node_types = node_type_get_types();

  // Check that the content type exists in the receiving instance.
  if (is_null($node_types[$node_content_type])) {

    drupal_set_message(t("The content type of the content being imported does not exist in the receiving site for content type of: %content_type", array('%content_type' => $node_content_type)), 'error');

    return FALSE;
  }

  // Here we are going to unset the description fields.  If the
  // description has been changed but everything else about the
  // content type is the same then we will assume that this is just an
  // edit to the description.
  unset($node_content_type_object_from_file->description);
  unset($node_types[$node_content_type]->description);

  if ($node_content_type_object_from_file != $node_types[$node_content_type]) {

    drupal_set_message(t("When the dataset was created the node content type was different from this site's node content type. Please manually compare the content type: %content_type", array('%content_type' => $node_content_type)), 'error');

    return FALSE;
  }

  // Loop through all the lines in the file and put the nid's into an array.
  // Then loop through all the current nodes (in the receiving instance)
  // and check the nid value.  If it doesn't match an nid from the dataset file
  // then delete the existing node in the receiving instance.  This will delete
  // test nodes which have been created.
  $line_number = 0;
  $handle = fopen($file_path, "r");
  if ($handle) {
    while (($buffer = fgets($handle)) !== FALSE) {

      $line_number++;
      if ($line_number > 1) {

        // @todo Here we will need to unencode the line extracted.
        $node_from_file = unserialize(base64_decode($buffer));
        $nids_from_file[] = $node_from_file->nid;
      }
    }

    if (!feof($handle)) {
      drupal_set_message(t("Unexpected error when reading file."), 'error');
    }
    fclose($handle);
  }

  // Loop through the existing nodes (for that content type) and if
  // they don't exist in the dataset then delete them. Here we are
  // going to be a bit simpler but harsher.  If a node of *any*
  // content type exists with the matching ID then delete it. And
  // we're going to allow the user running the import to be able to
  // delete any nodes so that the data keeps consistent.
  $results = db_query('SELECT nid FROM {node} WHERE type = :type', array(':type' => $node_content_type));

  foreach ($results as $row) {

    if (!in_array($row->nid, $nids_from_file)) {

      // Delete node as it does not exist in the dataset being
      // imported.
      node_delete($row->nid);
    }
  }

  // Here we are going to get a list of fields which are of type 'file' for the
  // content type selected.
  $fields_of_type_file = array();
  $fields_info = field_info_instances('node', $node_content_type);
  foreach ($fields_info as $field_name => $field_value) {
    $field_info = field_info_field($field_name);
    $type = $field_info['type'];

    if ($type == 'file' || $type == 'image') {

      $fields_of_type_file[] = $field_name;

    }
  }

  // Loop through all the lines in the dataset file and extract each
  // node line. If the receiving instance has a node with a matching
  // nid then update it - or if not then create a new node with the
  // same nid. We will use a batch function to process the lines from
  // the file.
  $batch = array(
    'operations' => array(
      array(
        'data_export_import_batch_import_dataset_lines',
        array($file_path, $fields_of_type_file),
      ),
    ),
    'finished' => 'data_export_import_batch_import_dataset_lines_finished',
    'title' => t('Processing Import of nodes from file Batch'),
    'init_message' => t('Batch of node importing starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Batch importing of nodes from file has encountered an error.'),
    'file' => drupal_get_path('module', 'data_export_import') . '/includes/profiles/nodes.inc',
  );

  batch_set($batch);

  // Since this is not called from a file then we need to call the
  // batch process function.
  batch_process('admin/config/system/data_export_import/nodes/import');

  return TRUE;
}

/**
 * Will import a dataset file.
 *
 * This function is specifically set up to be called by drush.
 *
 * @param string $file
 *   The dataset file which is being imported.
 *
 * @return bool
 *   TRUE if the import process ran without errors.
 */
function data_export_import_import_all_content_types_using_drush($file) {

  // Read the first line - decode the object and check that the content types
  // are an exact match.
  $file_path = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/nodes/" . $file;

  $handle = fopen($file_path, "r");
  if ($handle) {

    $content_type_line_from_file = fgets($handle);
    fclose($handle);
  }

  $node_content_type_object_from_file = unserialize(base64_decode($content_type_line_from_file));

  // Check that the content types exist and match.
  $node_content_type = $node_content_type_object_from_file->type;

  node_types_rebuild();
  $node_types = node_type_get_types();

  // Check that the content type exists in the receiving instance.
  if (is_null($node_types[$node_content_type])) {

    drupal_set_message(t("The content type of the content being imported does not exist in the receiving site for content type of: %content_type", array('%content_type' => $node_content_type)), 'error');

    return FALSE;
  }

  // Here we are going to unset the description fields.  If the
  // description has been changed but everything else about the
  // content type is the same then we will assume that this is just an
  // edit to the description.
  unset($node_content_type_object_from_file->description);
  unset($node_types[$node_content_type]->description);

  if ($node_content_type_object_from_file != $node_types[$node_content_type]) {

    drupal_set_message(t("When the dataset was created the node content type was different from this site's node content type. Please manually compare the content type: %content_type", array('%content_type' => $node_content_type)), 'error');

    return;
  }

  // Loop through all the lines in the file and put the nid's into an array.
  // Then loop through all the current nodes (in the receiving instance)
  // and check the nid value.  If it doesn't match an nid from the dataset file
  // then delete the existing node in the receiving instance.  This will delete
  // test nodes which have been created.
  $line_number = 0;
  $handle = fopen($file_path, "r");
  if ($handle) {
    while (($buffer = fgets($handle)) !== FALSE) {

      $line_number++;
      if ($line_number > 1) {

        // @todo Here we will need to unencode the line extracted.
        $node_from_file = unserialize(base64_decode($buffer));
        $nids_from_file[] = $node_from_file->nid;
      }
    }

    if (!feof($handle)) {
      drupal_set_message(t("Unexpected error when reading file."), 'error');
    }
    fclose($handle);

  }

  // Loop through the existing nodes (for that content type) and if
  // they don't exist in the dataset then delete them. NB - We've
  // used n.nid here as the db_rewrite_sql function will call a
  // link to access/grants tables and this could result in an
  // ambiguous field called 'nid'.
  $results = db_query('SELECT nid FROM {node} WHERE type = :type', array(':type' => $node_content_type));

  foreach ($results as $row) {

    if (!in_array($row->nid, $nids_from_file)) {

      // Delete node as it does not exist in the dataset being
      // imported.
      node_delete($row->nid);
    }
  }

  // Loop through all the lines in the dataset file and extract each
  // node line. If the receiving instance has a node with a matching
  // nid then update it - or if not then create a new node with the
  // same nid. We will use a batch function to process the lines from
  // the file.
  $fields_of_type_file = array();
  $fields_info = field_info_instances('node', $node_content_type);
  foreach ($fields_info as $field_name => $field_value) {
    $field_info = field_info_field($field_name);
    $type = $field_info['type'];

    if ($type == 'file' || $type == 'image') {

      $fields_of_type_file[] = $field_name;

    }
  }

  $batch = array(
    'operations' =>
    array(
      array(
        'data_export_import_batch_import_dataset_lines',
        array(
          $file_path,
          $fields_of_type_file,
        ),
      ),
    ),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Batch importing of nodes from file has encountered an error.'),
    'file' => drupal_get_path('module', 'data_export_import') . '/includes/profiles/nodes.inc',
  );

  $batch['progressive'] = FALSE;
  batch_set($batch);

  // Since this is not called from a file then we need to call the
  // batch process function.
  drush_backend_batch_process();

  return TRUE;
}

/**
 * Batch function to process lines from a dataset.
 *
 * This function will process one line at a time from a dataset file.
 * To stop the function from processing the first line each time we
 * will store the file pointer and then start the next file read from
 * that point.
 *
 * @param string $file_path
 *   File being processed.
 *
 * @return bool
 *   TRUE if the address is in a valid format, and FALSE if it isn't.
 */
function data_export_import_batch_import_dataset_lines($file_path, $fields_of_type_file, &$context) {

  if (!isset($context['sandbox']['progress'])) {

    $context['sandbox']['progress'] = 0;

    // Get the number of lines we will be processing.
    $line_count = 0;
    $handle = fopen($file_path, "r");

    if ($handle) {
      while (($buffer = fgets($handle)) !== FALSE) {

        $line_count++;
      }

      if (!feof($handle)) {
        drupal_set_message(t("Error: unexpected fgets() fail\n"), 'error');
      }
      fclose($handle);
    }
    $context['sandbox']['max'] = $line_count;
  }

  // Open the file for reading.
  $file_handle = fopen($file_path, 'r');

  // Check if file pointer position exists in the sandbox, and jump to
  // location in file.
  if (isset($context['sandbox']['file_pointer_position'])) {
    fseek($file_handle, $context['sandbox']['file_pointer_position']);
  }

  // Get file line from the file.
  $line_from_file = fgets($file_handle);

  // The first line in the dataset file is a the content type data -
  // so we will ignore that line.
  if ($context['sandbox']['progress'] != 0) {

    $dataset_node = unserialize(base64_decode($line_from_file));

    // Find if there is an existing node and see if it matches what is
    // in the dataset. Note that node_load() returns FALSE if it can't
    // find a node.
    $existing_node = node_load($dataset_node->nid);

    // If node_load returns FALSE then it was not able to find a node
    // with the nid - therefore a new node needs to be created.
    // Otherwise the existing node may need to be updated with the
    // data from the dataset.
    if ($existing_node == FALSE) {

      // Saving data for a new node is relatively straight forward.
      // We just call a modified version of the node_save() function.
      // The difficult part is dealing with attached files and files
      // uploaded via CCK fields. First we will loop through the
      // fields looking for CCK fields which contain file data.  We
      // will then save these files. This means that we can then use
      // the new file ID in the node data and when we save the node
      // the link to the CCK file will be working correctly.
      foreach ($fields_of_type_file as $file_field_key => $file_field_data) {

        // Here we are going to loop through all of the files which
        // may have been attached to this field.
        if (isset($dataset_node->{$file_field_data}[LANGUAGE_NONE])) {
          foreach ($dataset_node->{$file_field_data}[LANGUAGE_NONE] as $file_array_key => $file_array_data) {

            $file_data = base64_decode($file_array_data['data_export_import_file_data']);
            $dest = $file_array_data['uri'];

            // Actually save the file to the file system.
            $file_object = file_save_data($file_data, $dest, FILE_EXISTS_REPLACE);

            // Set the node object to use the returned fid value as this
            // may be from the original fid value.
            $dataset_node->{$file_field_data}[LANGUAGE_NONE][$file_array_key]['fid'] = $file_object->fid;
          }
        }
      }

      // This is the important line where we actually save the node.
      // The node should now have correct fid values to link to the
      // CCK fields which have already been saved - and after saving
      // this node we will be able to use the correct revision vid to
      // link the node to the Upload module files which we will be
      // saving next.
      //
      // Here will try to use the new node_save()
      // function in D7 which may allow us to save the node with its
      // current ID.
      //
      // One thing, we will set a field first to prevent
      // the node being saved as a new file which would presumably be
      // issued a new ID number.
      unset($dataset_node->vid);
      data_export_import_node_save_with_defined_id($dataset_node);

      // Here we wll re-create all of the comments which were exported into the
      // dataset.  First we will delete all existing comments for the node -
      // this should probably not be necessary but there may be excess comments/
      // replies which are attached to the node which need to be removed.
      // Remember that this module is designed to end up with identical data on
      // the receiving site as the sending site.
      if (module_exists('comment')) {

        $comments_result = db_query("SELECT cid FROM {comment} WHERE nid = :comment_node ORDER BY cid ASC", array(':comment_node' => $dataset_node->nid));
        foreach ($comments_result as $comment_row) {

          comment_delete($comment_row->cid);
        }

        if (property_exists($dataset_node, 'comments')) {
          foreach ($dataset_node->comments as $attached_comment) {

            $attached_comment->is_anonymous = 0;

            // Before we save the record we are going to create a placeholder record
            // with the defined id value.  That way - when the comment_save function
            // tries to save the comment with a specified cid value the underlying
            // functions will be able to update an existing record and will not fail
            // to carry out the SQL UPDATE statement.
            db_merge('comment')
              ->key(array('cid' => $attached_comment->cid))
              ->fields(array(
                  'thread' => "",
                ))
              ->execute();

            comment_submit($attached_comment);
            comment_save($attached_comment);
          }
        }
      }
    }

    else {

      // If the nodes don't match then update the existing node with
      // the data from the dataset. Certain fields can be unset
      // because they would be expected to be different. We will store
      // the existing dataset node because it will be needed when
      // updating the node.
      $existing_node_vid = $existing_node->vid;

      // The node may have had new revisions created since the dataset
      // was created so we'll unset all the data about revisions.
      unset($dataset_node->vid);
      unset($existing_node->vid);
      unset($dataset_node->log);
      unset($existing_node->log);
      unset($dataset_node->revision_timestamp);
      unset($existing_node->revision_timestamp);

      // The changed date for the existing node could easily be
      // different from the dataset node - even from a previous import
      // of the dataset - or say a user saved the node without any
      // changes.
      unset($dataset_node->changed);
      unset($existing_node->changed);

      // There is an important tweak RE comments attached to the node.  If there
      // were any comments attached to the original node then these are attached
      // to the node object as an array on the 'comments' attribute.  As the
      // existing_node will never have this attribute then the nodes will never
      // match and so the node will be updated.  This is fine as it saves us
      // from having to check that the comments from the existing node are an
      // exact match for the comments on the node being imported.  As for import
      // times - it is probably better to recreate the comments as checking
      // existing comments would probably add more overhead than simply updating
      // the node.
      if ($dataset_node != $existing_node) {

        // Create a new revision. The dataset node may be from several
        // revisions back - and we don't want to reset back to a
        // previous revision - and we don't want to overwrite the
        // current revision - so creating a new revision is the best
        // option. The only other option is to delete the node (which
        // would delete all revisions) and then re-create it - but
        // this would lose all the revision history.
        $dataset_node->vid = $existing_node_vid;
        $dataset_node->revision = 1;
        $dataset_node->log = "Imported from dataset file: " . $file_path;

        foreach ($fields_of_type_file as $file_field_key => $file_field_data) {

          // Here we are going to loop through all of the files which
          // may have been attached to this field and re-import them.
          if (isset($dataset_node->{$file_field_data}[LANGUAGE_NONE])) {
            foreach ($dataset_node->{$file_field_data}[LANGUAGE_NONE] as $file_array_key => $file_array_data) {

              $file_data = base64_decode($file_array_data['data_export_import_file_data']);
              $dest = $file_array_data['uri'];

              // Actually save the file to the file system.
              $file_object = file_save_data($file_data, $dest, FILE_EXISTS_REPLACE);

              // Set the node object to use the returned fid value as
              // this may be from the original fid value.
              $dataset_node->{$file_field_data}[LANGUAGE_NONE][$file_array_key]['fid'] = $file_object->fid;
            }
          }
        }

        node_save($dataset_node);

        // Here we wll re-create all of the comments which were
        // exported into the dataset.  First we will delete all
        // existing comments for the node - this should probably not
        // be necessary but there may be excess comments/replies which
        // are attached to the node which need to be removed. Remember
        // that this module is designed to end up with identical data
        // on the receiving site as the sending site.
        if (module_exists('comment')) {

          $comments_result = db_query("SELECT cid FROM {comment} WHERE nid = :comment_node ORDER BY cid ASC", array(':comment_node' => $dataset_node->nid));

          foreach ($comments_result as $comment_row) {
            comment_delete($comment_row->cid);
          }

          if (property_exists($dataset_node, 'comments')) {
            foreach ($dataset_node->comments as $attached_comment) {

              // This is needed to get the comment_save to work - not sure why.
              $attached_comment->is_anonymous = 0;

              // Before we save the record we are going to create a
              // placeholder record with the defined id value.  That way -
              // when the comment_save function tries to save the comment
              // with a specified cid value the underlying functions will
              // be able to update an existing record and will not fail to
              // carry out the SQL UPDATE statement.
              db_merge('comment')
                ->key(array('cid' => $attached_comment->cid))
                ->fields(array(
                    'thread' => "",
                  ))
                ->execute();

              comment_submit($attached_comment);
              comment_save($attached_comment);
            }
          }
        }
      }
    }
  }

  $context['sandbox']['progress']++;

  $context['results'][$file_path] = basename($file_path);

  // Retain current file pointer position.
  $context['sandbox']['file_pointer_position'] = ftell($file_handle);

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch 'finished' callback.
 */
function data_export_import_batch_import_dataset_lines_finished($success, $results, $operations) {
  if ($success) {

    // Here we do something meaningful with the results.
    $message = "The following file was imported:";
    $message .= theme('item_list', array('items' => $results));
  }
  else {

    // An error occurred. NB that $operations contains the operations
    // that remained unprocessed.
    $error_operation = reset($operations);
    $message = t(
      'An error occurred while processing %error_operation with arguments: @arguments',
      array(
        '%error_operation' => $error_operation[0],
        '@arguments' => print_r($error_operation[1], TRUE),
      )
    );
  }

  drupal_set_message($message);
}

/**
 * Batch function to save node with a defined id.
 *
 * This function is a modified version of the standard node save
 * function which allows for the id to be defined.
 *
 * @param object $node
 *   Node being saved.
 *
 * @return bool
 *   TRUE if function runs OK.
 */
function data_export_import_node_save_with_defined_id($node) {

  // Here we will remove the nid from the object to trick the rest of
  // the function into thinking it is dealing with a normal insert. We
  // will then re-introduce the nid when needed.
  $defined_nid = $node->nid;
  unset($node->nid);

  $transaction = db_transaction();

  try {
    // Load the stored entity, if any.
    if (!empty($node->nid) && !isset($node->original)) {
      $node->original = entity_load_unchanged('node', $node->nid);
    }

    field_attach_presave('node', $node);
    global $user;

    // Determine if we will be inserting a new node.
    if (!isset($node->is_new)) {
      $node->is_new = empty($node->nid);
    }

    // Set the timestamp fields.
    if (empty($node->created)) {
      $node->created = REQUEST_TIME;
    }

    // The changed timestamp is always updated for bookkeeping purposes,
    // for example: revisions, searching, etc.
    $node->changed = REQUEST_TIME;
    $node->timestamp = REQUEST_TIME;
    $update_node = TRUE;

    // Let modules modify the node before it is saved to the database.
    module_invoke_all('node_presave', $node);
    module_invoke_all('entity_presave', $node, 'node');

    if ($node->is_new || !empty($node->revision)) {
      // When inserting either a new node or a new node revision, $node->log
      // must be set because {node_revision}.log is a text column and therefore
      // cannot have a default value. However, it might not be set at this
      // point (for example, if the user submitting a node form does not have
      // permission to create revisions), so we ensure that it is at least an
      // empty string in that case.
      if (!isset($node->log)) {
        $node->log = '';
      }
    }
    elseif (!isset($node->log) || $node->log === '') {
      // If we are updating an existing node without adding a new revision, we
      // need to make sure $node->log is unset whenever it is empty. As long as
      // $node->log is unset, drupal_write_record() will not attempt to update
      // the existing database column when re-saving the revision; therefore,
      // this code allows us to avoid clobbering an existing log entry with an
      // empty one.
      unset($node->log);
    }

    // When saving a new node revision, unset any existing $node->vid so as to
    // ensure that a new revision will actually be created, then store the old
    // revision ID in a separate property for use by node hook implementations.
    if (!$node->is_new && !empty($node->revision) && $node->vid) {
      $node->old_vid = $node->vid;
      unset($node->vid);
    }

    // Save the node and node revision.
    if ($node->is_new) {

      // For new nodes, save new records for both the node itself and the node
      // revision. Just before we save the record we are going to
      // reset the nid - currently drupal_write_record() looks like it
      // respects an nid which has been sent in to it.
      $node->nid = $defined_nid;

      drupal_write_record('node', $node);
      _node_save_revision($node, $user->uid);
      $op = 'insert';
    }
    else {
      // For existing nodes, update the node record which matches the value of
      // $node->nid.
      drupal_write_record('node', $node, 'nid');
      // Then, if a new node revision was requested, save a new record for
      // that; otherwise, update the node revision record which matches the
      // value of $node->vid.
      if (!empty($node->revision)) {
        _node_save_revision($node, $user->uid);
      }
      else {
        _node_save_revision($node, $user->uid, 'vid');
        $update_node = FALSE;
      }
      $op = 'update';
    }
    if ($update_node) {
      db_update('node')
        ->fields(array('vid' => $node->vid))
        ->condition('nid', $node->nid)
        ->execute();
    }

    // Call the node specific callback (if any). This can be
    // node_invoke($node, 'insert') or
    // node_invoke($node, 'update').
    node_invoke($node, $op);

    // Save fields.
    $function = "field_attach_$op";
    $function('node', $node);

    module_invoke_all('node_' . $op, $node);
    module_invoke_all('entity_' . $op, $node, 'node');

    // Update the node access table for this node. There's no need to delete
    // existing records if the node is new.
    $delete = $op == 'update';
    node_access_acquire_grants($node, $delete);

    // Clear internal properties.
    unset($node->is_new);
    unset($node->original);
    // Clear the static loading cache.
    entity_get_controller('node')->resetCache(array($node->nid));

    // Ignore slave server temporarily to give time for the
    // saved node to be propagated to the slave.
    db_ignore_slave();
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('node', $e);
    throw $e;
  }

  return TRUE;
}
