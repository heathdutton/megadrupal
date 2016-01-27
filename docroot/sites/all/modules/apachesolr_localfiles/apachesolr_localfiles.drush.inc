<?php

/**
 * @file
 * Drush commands needed to search and index files.
 */

/**
 * Implements hook_drush_command().
 */
function apachesolr_localfiles_drush_command() {
  $commands = array();
  $commands['solr-local'] = array(
    'description' => t('Search for files changed after the last search'),
    'required-arguments' => 1,
    'arguments' => array('search type' => "Specify if it's the first search or an update search"),
  );
  return $commands;
}

/**
 * Callback for command search-updated-files.
 */
function drush_apachesolr_localfiles_solr_local() {

  if (func_get_arg(0) == 'first') { // first time importing the files
    $opt = '';
  }
  else if (func_get_arg(0) == 'update') { // updating the index with new files
    $last_index = variable_get('apachesolr_localfiles_last_index');
    if (!empty($last_index)) {
      $now = time();
      $difference_in_minutes = floor(($now - $last_index) / 60);
      $opt .= ' -mmin -' . $difference_in_minutes;
    }
  }
  else {
    print t('[ERROR] You passed the wrong parameters. The only two options are "first" and "update".') . "\n";
    return FALSE;
  }

  $folders_to_search = variable_get('apachesolr_localfiles_folder');
  $folders_to_search_formatted = '';
  foreach ($folders_to_search as $folder) {
    $folders_to_search_formatted .=  '"' . $folder['from'] . '" ';
  }
  
  print t('Starting to search on ') . $folders_to_search_formatted . '...' . "\n";

  $file_extensions_to_search = variable_get('apachesolr_localfiles_mimetypes_allowed');
  if (!empty($file_extensions_to_search)) {
    foreach ($file_extensions_to_search as $ext) {
      if (!empty($ext)) { // The settings form save option as zero
        $opt .= ' -name "*.' . $ext . '" -o';
      }
    }
    $opt = substr($opt, 0, -3);
  }

  $cmd = 'find ' . $folders_to_search_formatted . ' -type f' . $opt;

  $shell_return = shell_exec($cmd);

  if ($shell_return) {

    $files_array = explode("\n", $shell_return);
    array_pop($files_array);

    $solr_url = 'http://localhost:8983/solr';

    foreach ($files_array as $filepath) {

      $parameter_via_hooks = module_invoke_all('apachesolr_localfiles_indexing_parameter', $filepath);
      
      $id = transliteration_clean_filename($filepath);
      $cmd = 'curl "' . $solr_url . '/update/extract?commit=true&literal.id=' . $id;
      $cmd .= '&literal.filepath=' . urlencode($filepath);
      $cmd .= '&literal.filename=' . urlencode(basename($filepath));
      if (!empty($parameter_via_hooks)) {
        $cmd .= '&' . implode('&', $parameter_via_hooks);
      }
      $cmd .= '"';
      $cmd .= ' -F "file=@' . $filepath . '"';
      shell_exec($cmd);
    }

    variable_set('apachesolr_localfiles_last_index', time());
  }
  else {
    watchdog('error', t('apachesolr_localfiles had a problem indexing the files'));
  }
}