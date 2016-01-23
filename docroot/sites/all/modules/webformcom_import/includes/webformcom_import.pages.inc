<?php
/**
 * @file
 * Page callbacks for the Webform.com Import module.
 */

/**
 * Form callback; The main form for importing Webform.com exports.
 */
function webformcom_import_form($form, &$form_state) {
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('Import Webform module submissions and forms into your site from an export from a site export from <a href="http://webform.com">Webform.com</a>.') . '</p>',
  );

  $form['file'] = array(
    '#type' => 'textfield',
    '#title' => t('Export file location'),
    '#default_value' => webformcom_import_detect_export_zip(),
    '#description' => t('Specify the location of the Webform.com export file. This should include the entire path, from the root of the server. The file should still be compressed and have the name <em>webformcom_import_[id].zip</em> (where [id] is a site ID). This module will automatically detect files placed within the root of the site installation.'),
    '#attributes' => array('placeholder' => '/home/username/webformcom_export_12345.zip'),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Optional settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // TODO: Support command-line unzip for servers without the ZipArchive class.
  $zip_location = ''; //@exec('which unzip');
  $form['settings']['unzip_location'] = array(
    '#type' => 'value',
    '#title' => t('Unzip executable'),
    '#default_value' => $zip_location ? $zip_location : '/usr/bin/unzip',
    '#description' => t('On most systems, the <code>unzip</code> executable is located at <code>/usr/bin/zip</code>. If the zip executable is elsewhere on your server, enter its path here.'),
    '#access' => !class_exists('ZipArchive'),
  );

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['settings']['stream_wrapper'] = array(
    '#type' => 'radios',
    '#title' => t('File handling'),
    '#options' => $scheme_options,
    '#description' => t('All files on Webform.com use the private file handling method, but when importing files into your own site you may choose to use the public file handling method, which is faster and more compatible, but less secure.'),
    '#default_value' => variable_get('file_default_scheme', 'public'),
    '#access' => count($scheme_options) > 1,
  );

  $form['settings']['renumber_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Renumber IDs'),
    '#description' => t('If checked, all imported forms (nodes), files, and submissions will start at the next available ID on your site. This should be done unless working on a fresh database and maintaining exact IDs is important (such as if attempting to maintain URL consistency with Webform.com).'),
    '#default_value' => TRUE,
  );

  // Only allow this option to be changed if the site does not yet have content.
  if (webformcom_import_content_exists()) {
    $form['settings']['renumber_ids']['#value'] = TRUE;
    $form['settings']['renumber_ids']['#disabled'] = TRUE;
    $form['settings']['renumber_ids']['#description'] .= ' <strong>' . t('This option is disabled because your site already has content with which this import may conflict.') . '</strong>';
  }

  $form['settings']['cleanup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete export file after completion'),
    '#description' => t('For security of your imported information, the export file should be removed after the import is complete.'),
    '#default_value' => TRUE,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Begin import'),
    // Explicitly set these properties for the use of the installer.
    '#validate' => array('webformcom_import_form_validate'),
    '#submit' => array('webformcom_import_form_submit'),
  );

  return $form;
}

/**
 * Simple check if the site has any created content.
 */
function webformcom_import_content_exists() {
  static $content_created;
  if (!isset($content_created)) {
    $content_nid = db_query("SELECT MAX(nid) FROM {node}")->fetchCol();
    $content_created = $content_nid[0] > 10;
  }
  return $content_created;
}

/**
 * Validate handler for webformcom_import_form().
 */
function webformcom_import_form_validate($form, &$form_state) {
  // Check that the import file exists.
  if (!file_exists($form_state['values']['file'])) {
    form_error($form['file'], t('The export file could not be found at this location. Check that the file exists.'));
  }

  // Check that file directories are writable.
  $directories = array(
    'temporary://',
    $form_state['values']['stream_wrapper'] . '://webform',
  );
  foreach ($directories as $directory) {
    if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      $wrapper = file_stream_wrapper_get_instance_by_uri($directory);
      form_set_error('', t('The directory @path is not writable. Check that your <a href="!file-system">file system settings</a> are correct and the directory is writable.', array('@path' => $wrapper->realpath(), '!file-system' => url('admin/config/media/file-system'))));
    }
  }

  // The Webform module should be enabled anyway based on dependencies, but
  // double-check anyway.
  if (!module_exists('webform')) {
    form_set_error('', t('The <a href="http://drupal.org/project/webform">Webform module</a> must be enabled before the import can be run.'));
  }

  // Check that some form of unzip is available.
  if (!class_exists('ZipArchive')) {
    if (!is_executable($form_state['unzip_location'])) {
      form_error($form['settings']['unzip_location'], t('The unzip executeable either does not exist or is not executable. Check that the unzip is installed on your server.'));
    }
    elseif (!function_exists('exec')) {
      form_set_error($form['settings']['unzip_location'], t('The unzip executeable will not be able to function because the exec() command is not available on your server. Check that you do not have PHP "safe_mode" enabled or have other security software installed that prevents use of exec().'));
    }
  }

}

/**
 * Submit handler for webformcom_import_form().
 */
function webformcom_import_form_submit($form, &$form_state) {
  $settings = array(
    'file' => $form_state['values']['file'],
    'stream_wrapper' => $form_state['values']['stream_wrapper'],
    'renumber_ids' => $form_state['values']['renumber_ids'],
    'cleanup' => $form_state['values']['cleanup'],
  );
  $batch = webformcom_import_batch($settings);
  batch_set($batch);
}

/**
 * Start a Batch operation to import a site archive.
 */
function webformcom_import_batch($settings) {
  // Set defaults.
  $settings += array(
    'stream_wrapper' => 'public',
    'renumber_ids' => TRUE,
    'cleanup' => TRUE,
  );

  $batch = array(
    'title' => t('Importing site'),
    'init_message' => t('Starting import...'),
    'error_message' => t('An error occurred during the import. Please <a href="mailto:support@webform.com">contact support</a> if this problem continues.'),
    'file' => drupal_get_path('module', 'webformcom_import') . '/includes/webformcom_import.pages.inc',
    'operations' => array(),
    'finished' => 'webformcom_import_batch_finished',
  );
  $batch['operations'][] = array('webformcom_import_batch_step1_copy_archive', array($settings));
  $batch['operations'][] = array('webformcom_import_batch_step2_expand_archive', array($settings));
  $batch['operations'][] = array('webformcom_import_batch_step3_import_database', array($settings));
  $batch['operations'][] = array('webformcom_import_batch_step4_import_nodes', array($settings));
  $batch['operations'][] = array('webformcom_import_batch_step5_import_submissions', array($settings));
  $batch['operations'][] = array('webformcom_import_batch_step6_import_files', array($settings));
  $batch['operations'][] = array('webformcom_import_batch_step7_cleanup', array($settings));

  array_unshift($batch['operations'], array('webformcom_import_batch_step0_setup', array($batch['operations'])));

  return $batch;
}

/**
 * Initial setup to validate everything we can know beforehand.
 */
function webformcom_import_batch_step0_setup($operations, &$context) {
  $context['message'] = t('Starting import. Backing up import archive...');
}

/**
 * Back up the archive before decompressing it.
 */
function webformcom_import_batch_step1_copy_archive($settings, &$context) {
  $destination_uri = 'temporary://' . basename($settings['file']);
  if (file_exists($destination_uri)) {
    unlink($destination_uri);
  }
  file_unmanaged_copy($settings['file'], $destination_uri);
  $context['message'] = t('Expanding archive...');
}

/**
 * Decompress the archive.
 */
function webformcom_import_batch_step2_expand_archive($settings, &$context) {
  $source_uri = 'temporary://' . basename($settings['file']);
  $source_wrapper = file_stream_wrapper_get_instance_by_uri($source_uri);
  $source_path = $source_wrapper->realpath();
  $target_directory = dirname($source_path) . '/' . basename($source_path, '.zip') . '/';

  $zip = new ZipArchive;
  $zip->open($source_path);
  $zip->extractTo($target_directory);
  $zip->close();
}

/**
 * Import the database into temporary tables.
 */
function webformcom_import_batch_step3_import_database($settings, &$context) {
  $source_uri = 'temporary://' . basename($settings['file']);
  $source_wrapper = file_stream_wrapper_get_instance_by_uri($source_uri);
  $source_path = $source_wrapper->realpath();
  $base_file = basename($source_path, '.zip');
  $source_sql_file = dirname($source_path) . '/' . $base_file . '/' . $base_file . '.sql';

  _webformcom_import_read_dump($source_sql_file);
}

/**
 * Import individual nodes.
 */
function webformcom_import_batch_step4_import_nodes($settings, &$context) {
  if (!isset($context['sandbox']['position'])) {
    $context['sandbox']['position'] = 0;
  }

  $count = 50; // Node rows to process at a time.
  $result = db_query_range("SELECT * FROM webformcom_export_node ORDER BY nid ASC", $context['sandbox']['position'], $count);

  foreach ($result as $row) {
    $node = (object) (array) $row;
    $node->is_new = TRUE;
    $node->uid = !empty($GLOBAL['user']->uid) ? $GLOBAL['user']->uid : 1;
    unset($node->vid);
    $old_nid = $node->nid;
    if ($settings['renumber_ids']) {
      unset($node->nid);
    }
    node_save($node);
    $nid = $node->nid;

    // Migrate the Webform data itself (but not submissions yet).
    $tables = db_query("SHOW TABLES LIKE 'webformcom_export_webform%'")->fetchCol();
    $excluded_tables = array(
      'webformcom_export_webform_submissions',
      'webformcom_export_webform_submitted_data',
      // Webform roles are always left at the default.
      'webformcom_export_webform_roles',
      // TODO: Upgrade Webform Validation if it becomes enabled on webform.com.
      'webformcom_export_webform_validation_rule',
      'webformcom_export_webform_validation_rule_components',
    );
    $tables = array_diff($tables, $excluded_tables);
    foreach ($tables as $tablename) {
      $table_result = db_query("SELECT * FROM " . db_escape_table($tablename) . " WHERE nid = :nid", array(':nid' => $old_nid));
      foreach ($table_result as $table_row) {
        $table_row = (array) $table_row;
        $table_row['nid'] = $nid;
        $target_tablename = str_replace('webformcom_export_', '', $tablename);
        // Rows will already exist for the 'webform' table so they need an
        // updated, indicated by the 3rd parameter to drupal_write_record().
        if ($target_tablename === 'webform' && in_array($node->type, webform_variable_get('webform_node_types_primary'))) {
          drupal_write_record($target_tablename, $table_row, array('nid' => $nid));
        }
        // Everything else is always an INSERT into the appropriate table.
        else {
          drupal_write_record($target_tablename, $table_row);
        }
      }
    }

    // Store NID map in the results array so we can replace other NIDs in later
    // steps.
    $context['results']['nodes'][$old_nid] = $nid;
    $context['sandbox']['position']++;
  }

  if (empty($context['sandbox']['max']) || $context['sandbox']['position'] === $context['sandbox']['max']) {
    $context['finished'] = 1;
    $context['message'] = t('Staring to import files.');
  }
  else {
    $context['finished'] = $context['sandbox']['position'] / $context['sandbox']['max'];
    $context['message'] = t('Imported @count of @total forms.', array('@count' => $context['sandbox']['position'], '@total' => $context['sandbox']['max']));
  }
}

/**
 * Import the submissions.
 */
function webformcom_import_batch_step5_import_submissions($settings, &$context) {
  if (!isset($context['sandbox']['position'])) {
    $context['sandbox']['position'] = 0;
    $context['sandbox']['max'] = db_query("SELECT COUNT(*) FROM {webformcom_export_webform_submissions}");
  }

  $count = 200; // Submission rows to process at a time.
  $result = db_query_range("SELECT * FROM webformcom_export_webform_submissions ORDER BY sid ASC", $context['sandbox']['position'], $count);

  foreach ($result as $row) {
    // Insert the submission.
    $submission_row = (array) $row;
    $old_nid = $submission_row['nid'];
    $old_sid = $submission_row['sid'];
    $nid = $context['results']['nodes'][$old_nid];
    $submission_row['nid'] = $nid;

    if ($settings['renumber_ids']) {
      unset($submission_row['sid']);
    }
    $sid = db_insert('webform_submissions')
      ->fields($submission_row)
      ->execute();

    // Migrate the submission data.
    $data_result = db_query("SELECT * FROM webformcom_export_webform_submitted_data WHERE sid = :sid", array(':sid' => $old_sid));
    foreach ($data_result as $data_row) {
      $data_row = (array) $data_row;
      $data_row['nid'] = $nid;
      $data_row['sid'] = $sid;
      db_insert('webform_submitted_data')
        ->fields($data_row)
        ->execute();
    }

    // Store SID map in the results array so we can replace other SIDs in later
    // steps.
    $context['results']['submissions'][$old_sid] = $sid;
    $context['sandbox']['position']++;
  }

  if (empty($context['sandbox']['max']) || $context['sandbox']['position'] === $context['sandbox']['max']) {
    $context['finished'] = 1;
    $context['message'] = t('Staring to import files.');
  }
  else {
    $context['finished'] = $context['sandbox']['position'] / $context['sandbox']['max'];
    $context['message'] = t('Imported @count of @total forms.', array('@count' => $context['sandbox']['position'], '@total' => $context['sandbox']['max']));
  }
}

/**
 * Convert the file records and move files into the final location.
 */
function webformcom_import_batch_step6_import_files($settings, &$context) {
  if (!isset($context['sandbox']['max'])) {
    $context['sandbox']['max'] = db_query("SELECT COUNT(*) FROM {webformcom_export_file_managed}");
    $context['sandbox']['position'] = 0;
  }

  // The temporary directory name is based on the original zip file containing
  // the entire site export. This results in the location of the individual
  // files all being within temporary://site_name_123456/private/.
  $base_file = basename($settings['file'], '.zip');
  $source_directory_uri = 'temporary://' . $base_file . '/private';

  $count = 500; // File rows to process at a time.
  $result = db_query_range("SELECT * FROM {webformcom_export_file_managed} ORDER BY fid ASC", $context['sandbox']['position'], $count);

  // Check if the type column exists, which is needed for file entity support.
  $type_column_exists = db_field_exists('file_managed', 'type');

  foreach ($result as $file) {
    $context['sandbox']['position']++;

    $file = (array) $file;
    $old_fid = $file['fid'];
    $old_uri = $file['uri'];

    // Renumber the file ID if needed.
    if ($settings['renumber_ids']) {
      unset($file['fid']);
    }

    // All files by default contain the node ID, replace in the file path.
    if (preg_match('/^private:\/\/webform\/[^\/]+\/(\d+)\//', $file['uri'], $matches)) {
      $nid_in_uri = $matches[1];
      $new_nid = $context['results']['nodes'][$nid_in_uri];
      $file['uri'] = str_replace("/$nid_in_uri/", "/$new_nid/", $file['uri']);
    }

    // Change the file location to be public if needed.
    if ($settings['stream_wrapper'] !== 'private') {
      $file['uri'] = str_replace('private://', $settings['stream_wrapper'] . '://', $file['uri']);
    }

    // File entity support, add the "type" column data.
    if ($type_column_exists) {
      $file['type'] = substr($file['filemime'], 0, strpos($file['filemime'], '/'));
    }

    // Move the file to its new location. We use rename for speed instead of
    // file_unmanaged_move(), because it actually uses a copy internally.
    $source_uri = $source_directory_uri . '/' . str_replace($settings['stream_wrapper'] . '://', '', $old_uri);
    rename($source_uri, $file['uri']);

    // Insert the new file record.
    $fid = db_insert('file_managed')
      ->fields($file)
      ->execute();

    // Insert matching file usages.
    $file_usages = db_query("SELECT * FROM {webformcom_export_file_usage} WHERE fid = :fid", array(':fid' => $old_fid));
    foreach ($file_usages as $file_usage) {
      $file_usage = (array) $file_usage;
      $file_usage['fid'] = $fid;

      // Convert the ID for files.
      if ($file_usage['module'] == 'node' && $file_usage['type'] == 'node') {
        $file_usage['id'] = $context['results']['nodes'][$file_usage['id']];
      }
      if ($file_usage['module'] == 'webform' && $file_usage['type'] == 'submission') {
        $file_usage['id'] = $context['results']['submissions'][$file_usage['id']];
      }

      // Insert the file usage record.
      db_insert('file_usage')
        ->fields($file_usage)
        ->execute();
    }

    // Update existing submission file IDs.
    $data_results = db_query("SELECT wsd.nid, wsd.sid, wsd.cid, wsd.no FROM {webform_submitted_data} wsd INNER JOIN {webform_component} wc ON wsd.nid = wc.nid AND wsd.cid = wc.cid AND wc.type = :file WHERE wsd.data = :fid", array(':file' => 'file', ':fid' => $old_fid));
    foreach ($data_results as $data_row) {
      db_update('webform_submitted_data')
        ->fields(array('data' => $fid))
        ->condition('nid', $data_row->nid)
        ->condition('sid', $data_row->sid)
        ->condition('cid', $data_row->cid)
        ->condition('no', $data_row->no)
        ->execute();
    }
  }
}

/**
 * Delete all the temporary tables, files, etc.
 */
function webformcom_import_batch_step7_cleanup($settings, &$context) {
  // Delete the original file if requested.
  if ($settings['cleanup']) {
    if (is_file($settings['file']) && !@unlink($settings['file'])) {
      drupal_set_message(t('The import file @file could not be automatically deleted. Please remove it manually from your server.', array('@file' => $settings['file'])), 'warning');
    }
  }

  // Always delete the temporary copies.
  $source_uri = 'temporary://' . basename($settings['file']);
  $source_wrapper = file_stream_wrapper_get_instance_by_uri($source_uri);
  $source_path = $source_wrapper->realpath();
  $base_file = basename($source_path, '.zip');
  $source_directory = dirname($source_path) . '/' . $base_file;

  file_unmanaged_delete($source_uri);
  file_unmanaged_delete_recursive($source_directory);
}

/**
 * Batch API completion callback.
 */
function webformcom_import_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('Import complete!') . ' ' . format_plural(count($results['nodes']), '1 form imported.', '@count forms imported.') . ' ' . format_plural(count($results['submissions']), '1 submission imported.', '@count submissions imported.'));
    // Don't redirect during the installer.
    if (!defined('MAINTENANCE_MODE')) {
      drupal_goto('admin/config/services/webformcom-import/complete');
    }
  }
}

/**
 * Menu callback; Show the completion page.
 */
function webformcom_import_complete_page() {
  $output = array();
  $output['title']['#markup'] = '<h2>' . t('What now?') . '</h2>';
  $output['item_list']['#theme'] = 'item_list';
  $output['item_list']['#items'] = array(
    t('First you should check that your imported forms came in okay. Check them out at the <a href="!webform-list">Webform content list</a>.', array('!webform-list' => url('admin/content/webform'))),
    t('After confirming your import, you can disable the Webform.com Import module using the <a href="!modules">Modules page</a>, or by using the button below.', array('!modules' => url('admin/modules'))),
  );
  $output['disable_form'] = drupal_get_form('webformcom_import_disable');
  return $output;
}

/**
 * Form to disable the webformcom import module.
 */
function webformcom_import_disable() {
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['disable'] = array(
    '#type' => 'submit',
    '#value' => t('Disable Webform.com Import'),
  );
  $form['actions']['back'] = array(
    '#type' => 'link',
    '#href' => 'admin/config/services/webformcom-import',
    '#title' => t('Back to import page'),
  );
  return $form;
}

/**
 * Submit handler for webformcom_import_disable().
 */
function webformcom_import_disable_submit($form, &$form_state) {
  module_disable(array('webformcom_import'), FALSE);
  drupal_uninstall_modules(array('webformcom_import'), FALSE);
  drupal_flush_all_caches();
  drupal_set_message('Webform.com Import has been disabled and uninstalled. You may now delete it from your modules directory. Thanks for using Webform.com!');
  drupal_goto('admin/config');
}

/**
 * Search the Drupal root for an export file.
 */
function webformcom_import_detect_export_zip() {
  $path = '';
  $root = getcwd();
  $matches = file_scan_directory($root, '/^webformcom_export_[0-9]+(.*)\.zip(.*)$/', array('recurse' => FALSE));
  if ($matches) {
    $first_match = reset($matches);
    $path = $first_match->uri;
  }
  return $path;
}

/**
 * Backup the databases to a file.
 */
function _webformcom_import_read_dump($file_path) {
  $num = 0;
  $file = fopen($file_path, 'r');
  $connection = Database::getConnection();
  // Read one line at a time and run the query.
  while ($line = _webformcom_import_read_single_command($file)) {
    if ($line) {
      // Prepare and execute the statement instead of the api function to avoid
      // substitution of '{' etc.
      $statement = $connection->prepare($line);
      $statement->execute();
    }
  }
  fclose($file);
}

/**
 * Read a multiline sql command from a file.
 *
 * Supports the formatting created by mysqldump, but won't handle multiline
 * comments.
 */
function _webformcom_import_read_single_command($file) {
  $out = '';
  while ($line = fgets($file)) {
    $first2 = substr($line, 0, 2);
    $first3 = substr($line, 0, 2);

    // Ignore single line comments. This function doesn't support multiline comments or inline comments.
    if ($first2 != '--' && ($first2 != '/*' || $first3 == '/*!')) {
      $out .= ' ' . trim($line);
      // If a line ends in ; or */ it is a sql command.
      if (substr($out, strlen($out) - 1, 1) == ';') {
        return trim($out);
      }
    }
  }
  return trim($out);
}

