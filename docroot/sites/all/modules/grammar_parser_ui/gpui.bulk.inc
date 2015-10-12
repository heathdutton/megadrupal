<?php

/**
 * @file
 * Bulk parse routine file for the Grammar Parser UI module.
 *
 * Copyright 2009-10 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Sets batch operation items.
 *
 * @param array $extensions
 *   Array of file types to parse based on extension.
 * @param array $items
 *   Array of directories containing the files to parse.
 * @param boolean $recursive
 *   Indicates whether to recurse the subdirectories of each $item.
 * @param boolean $rewrite
 *   Indicates whether to rewrite each file.
 * @return boolean
 *   Indicates whether the batch operations were successfully set.
 */
function gpui_bulk_batch_set($extensions, $items, $recursive = TRUE, $rewrite = TRUE) {
  // Declare global variables.
  global $_gpui_log, $_gpui_debug;

  // Check lists in case this function is called apart from form submit.
  if (!is_array($extensions) || empty($extensions)) {
    return FALSE;
  }
  if (!is_array($items) || empty($items)) {
    return FALSE;
  }

  $_gpui_log = TRUE;
  if ($_gpui_log) {
    // Clear the log file.
    gpui_path_clear('log');
    gpui_path_clear('memory');
    gpui_log_memory_use('initial');
  }
  // Set debug output preference.
  $_gpui_debug = variable_get('gpui_enable_debug_output', FALSE);
  if ($_gpui_debug) {
    // Clear the debug file.
    // NOTE: When this module is enabled, the debug path is the same for this
    // module and the grammar parser module. In other words,
    // gpui_path('debug') == $reader->debugPath().
    gpui_path_clear('debug');
  }

  $batch = array(
    'title' => t('Bulk parsing of files'),
    'init_message' => t('Bulk parsing batch is starting.'),
    'progress_message' => t('Processed @current out of @total directories.'),
    'error_message' => t('Bulk parsing batch has encountered an error.'),
    'file' => drupal_get_path('module', 'gpui') . '/gpui.bulk.inc',
    'finished' => 'gpui_bulk_batch_finished',
  );
  // Create an operation for each user input directory.
  foreach ($items as $item) {
    $batch['operations'][] = array('gpui_bulk_batch_process', array($extensions, $item, $recursive, $rewrite));
  }
  batch_set($batch);
  return TRUE;
}

/**
 * Processes a batch operation.
 *
 * @param array $extensions
 *   Array of file types to parse based on extension.
 * @param array $item
 *   Array of the directory containing the files to parse.
 * @param boolean $recursive
 *   Indicates whether to recurse the subdirectories of each $item.
 * @param boolean $rewrite
 *   Indicates whether to rewrite each file.
 * @param array $context
 *   Array of batch context variables.
 */
function gpui_bulk_batch_process($extensions, $item, $recursive, $rewrite, &$context) {
  // Declare global variables.
  global $_gpui_log, $_gpui_debug;

  $_gpui_log = TRUE;
  // Set debug output preference.
  $_gpui_debug = variable_get('gpui_enable_debug_output', FALSE);

  if (!isset($context['sandbox']['progress'])) {
    // Initialize the batch context variables.
    gpui_bulk_batch_initialize($item, $recursive, $context);
    if ($context['sandbox']['max'] == 0) {
      $context['finished'] = 1;
      return;
    }
  }

  // Load grammar parser code.
  gpui_bulk_load_parser();

  $limit = $context['sandbox']['limit'];
  $i = 0;
  while (TRUE) {
    // Grab the first directory item.
    $item = array_shift($context['sandbox']['files']);
    if (is_null($item)) {
      break;
    }
    $dirname = $item['old_dir'];
    $new_dirname = $item['new_dir'];
    $patch_dirname = $item['patch_dir'];

    if (!isset($item['initialize'])) {
      gpui_log_print("\n*************************");
      gpui_log_print('Parsing the directory => ' . $dirname);
      gpui_log_print("*************************");
      // Make output directories.
      gpui_make_directory($new_dirname);
      gpui_make_directory($patch_dirname);
      $item['initialize'] = TRUE;
    }

    if (!isset($item['entries'])) {
      // Check here after creating any directories needed for subsequent items.
      continue;
    }

    // Process files.
    foreach ($item['entries'] as $index => $filename) {
      if (in_array(pathinfo($filename, PATHINFO_EXTENSION), array_keys($extensions))) {
        gpui_log_print("\n*************************");
        gpui_log_print('Parsing the file => ' . $filename);
        gpui_log_print("*************************");
        gpui_parse_file($dirname . '/' . $filename, $new_dirname . '/' . $filename, $rewrite);
        if ($rewrite) {
          // Make a patch file.
          gpui_make_patch_file($item, $filename);
        }
        $context['results'][] = $filename;
      }
      else {
        copy($dirname . '/' . $filename, $new_dirname . '/' . $filename);
      }
      unset($item['entries'][$index]);
      // Update the progress count.
      $context['sandbox']['progress']++;
      $i++;
      if ($i == $limit) {
        // This round is done.
        if (!empty($item['entries'])) {
          // Return item to list so remaining files are processed on next call.
          array_unshift($context['sandbox']['files'], $item);
        }
        $context['message'] = t('Processed @current out of @total files.', array('@current' => $context['sandbox']['progress'], '@total' => $context['sandbox']['max']));
        if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
          $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
        }
        return;
      }
    }
  }
  $context['message'] = t('Processed @current out of @total files.', array('@current' => $context['sandbox']['progress'], '@total' => $context['sandbox']['max']));
  $context['finished'] = 1;
}

/**
 * Initializes the batch context variables.
 *
 * @param array $item
 *   Array of the directory containing the files to parse.
 * @param boolean $recursive
 *   Indicates whether to recurse the subdirectories of each $item.
 * @param array $context
 *   Array of batch context variables.
 */
function gpui_bulk_batch_initialize($item, $recursive, &$context) {
  // Get list of files to parse.
  $count = 0;
  $files = gpui_bulk_scan_directory($item, $recursive, $count, 'files');

  // Initialize sandbox variables.
  $context['sandbox']['progress'] = 0;
  $context['sandbox']['files'] = $files;
  $context['sandbox']['max'] = $count;
  if ($context['sandbox']['max'] == 0) {
    $context['finished'] = 1;
    return;
  }
  // Set limit on number of files to process at a time.
  if ($context['sandbox']['max'] > 300) {
    $context['sandbox']['limit'] = 15;
  }
  elseif ($context['sandbox']['max'] > 200) {
    $context['sandbox']['limit'] = 10;
  }
  elseif ($context['sandbox']['max'] > 100) {
    $context['sandbox']['limit'] = 7;
  }
  else {
    $context['sandbox']['limit'] = 5;
  }/*
  elseif ($context['sandbox']['max'] > 50) {
    $context['sandbox']['limit'] = 3;
  }
  elseif ($context['sandbox']['max'] > 20) {
    $context['sandbox']['limit'] = 2;
  }
  elseif ($context['sandbox']['max'] > 0) {
    $context['sandbox']['limit'] = 1;
  }*/
}

/**
 * Processes the results of batch operations.
 */
function gpui_bulk_batch_finished($success, $results, $operations) {
  $message = format_plural(count($results), 'One file processed.', '@count files processed.');
  drupal_set_message($message);

  if (!$success) {
    $message = t('Finished with an error.');
    drupal_set_message($message);
  }
}

/**
 * Scans a directory and finds all files and/or directories beneath it.
 *
 * Returns a flat array structure where each directory is a top-level item.
 * This format allows us to create the output directories (for the new and
 * patch files) as we go.
 *
 * @param array $item
 *   Array of the directory containing the files to parse.
 * @param integer $count
 *   Number of files in the directory to parse.
 * @param string $retrieve
 *   Indicates what to return: directories only or files.
 * @return array
 *   Array of file and/or directory names.
 */
function gpui_bulk_scan_directory($item, $recursive, &$count, $retrieve = 'dirs') {
  $dirs = array();
  if (!is_dir($item['old_dir'])) {
    return $dirs;
  }

  static $ignore = array(/*'.', '..', '.bzr', '.git', '.svn',*/ 'CVS');

  $dirname = $item['old_dir'];
  $new_dirname = $item['new_dir'];
  $patch_dirname = $item['patch_dir'];

  $files[$dirname] = $item;
  $filenames = scandir($dirname);
  foreach ($filenames as $filename) {
    if (substr(basename($filename), 0, 1) == '.' || in_array(basename($filename), $ignore)) {
      // Ignore all hidden directories and CVS directory.
      continue;
    }
    elseif (is_dir($dirname . '/' . $filename) && $recursive) {
      $new_item = array(
        'name' => $filename,
        'old_dir' => $dirname . '/' . $filename,
        'new_dir' => $new_dirname . '/' . $filename,
        'patch_dir' => $patch_dirname . '/' . $filename,
      );
      // Merge subdirectories in after the files in the directory.
      // This orders the directories so we can create the new directories.
      $files = array_merge($files, gpui_bulk_scan_directory($new_item, $recursive, $count, $retrieve));
      $dirs[] = $filename;
    }
    else {
      $files[$dirname]['entries'][] = $filename;
      $count++;
    }
  }
  return $retrieve == 'dirs' ? $dirs : $files;
}

/**
 * Loads grammer parser code files.
 */
function gpui_bulk_load_parser() {
  libraries_load('grammar_parser');
}

/**
 * Parses a file.
 *
 * @param string $old_filename
 *   The name of the file to parse.
 * @param array $new_filename
 *   The name of the rewritten parsed file.
 * @param boolean $rewrite
 *   Indicates whether to rewrite each file.
 */
function gpui_parse_file($old_filename, $new_filename, $rewrite = TRUE) {
  if (!file_exists($old_filename)) {
    return FALSE;
  }
  // Avoid parsing a file multiple times.
  if (file_exists($new_filename)) {
    return TRUE;
  }

  // Read the file.
  $contents = file_get_contents($old_filename);

  // Create reader object.
  $reader = PGPReader::getInstance();
  gpui_log_memory_use('create reader for file ' . $old_filename);

  // Set array formatting preference.
  $reader->setPreserveArrayFormat(variable_get('gpui_preserve_array_format', FALSE));
  // Set debug output preference.
  $reader->setDebug(variable_get('gpui_enable_debug_output', FALSE));

  $reader->setSnippet($contents);
  gpui_log_memory_use('set snippet');
  unset($contents);
  gpui_log_memory_use('unset contents');
  $reader->addTokenNames();
  gpui_log_memory_use('add token names');
  // Parse code file.
  $reader->buildGrammar();
  gpui_log_memory_use('build grammar');

  if ($rewrite) {
    // Use writer to redo file.
    $writer = PGPWriter::getInstance();
    gpui_log_memory_use('create writer');
    $new = $writer->toString($reader->getStatements());
    gpui_log_memory_use('writer->toString');

    // Write the new file.
    if (file_put_contents($new_filename, $new) === FALSE) {
      gpui_log_print('File could not be written');
    }
    gpui_log_print('Replaced the file');
  }

  // Free up memory.
  $reader->reset();
  gpui_log_memory_use('reset reader');
}

/**
 * Makes a directory to write output files to.
 *
 * @param string $dirname
 *   The name of the directory to create.
 * @return boolean ???
 */
function gpui_make_directory($dirname) {
  // Create an output directory we can write to.
  if (!is_dir($dirname)) {
    mkdir($dirname);
    chmod($dirname, 0757);
  }
}

/**
 * Makes a patch file of the parser routine changes.
 *
 * @param array $item
 *   Array of the directory containing the files to parse.
 * @param string $filename
 *   The name of the file that was parsed.
 */
function gpui_make_patch_file($item, $filename) {
  // Make a patch file.
  gpui_log_print("\n*************************");
  gpui_log_print('Creating a patch file for => ' . $item['old_dir'] . '/' . $filename);
  gpui_log_print("*************************");
  $old_filename = $item['old_dir'] . '/' . $filename;
  $new_filename = $item['new_dir'] . '/' . $filename;
  $patch_filename = $item['patch_dir'] . '/' . $filename . '.patch';
  $command = "diff -up -w {$old_filename} {$new_filename} > {$patch_filename}";
  gpui_log_print("Making patch file: $command");
  shell_exec($command);

  // Remove the path strings from the patch file (for usability purposes).
  $old1 = $item['old_dir'] . '/';
  $new1 = $item['new_dir'] . '/';
  $contents = file_get_contents($patch_filename);
  file_put_contents($patch_filename, str_replace(array($old1, $new1), '', $contents));

  // Log whether any changes.
  $contents = file_get_contents($patch_filename);
  $status = (($contents === '') ? 'MATCH' : 'DELTA') . " => $new_filename\n";
  $patch_file = str_replace('log.txt', 'patch.log', gpui_path('log'));
  file_put_contents($patch_file, $status, FILE_APPEND);
}
