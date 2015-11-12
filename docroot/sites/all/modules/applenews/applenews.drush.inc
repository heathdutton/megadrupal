<?php
/**
 * @file
 * Drush hook implementations.
 */

/**
 * Implements hook_drush_command().
 */
function applenews_drush_command() {

  $items['applenews'] = [
    'description' => 'Export entities to Apple News.',
    'arguments' => [
      'op'           => 'Operation export/preview',
      'module'       => 'Apple News Export module name',
      'machine_name' => 'Apple News Export machine name',
    ],
    'examples' => [
      'applenews export applenews_style1 node'
        => 'Run export.',
      'applenews preview applenews_style1 node'
        => 'Run export to file.',
    ],
    'aliases' => ['an'],
  ];

  $items['applenews-libraries'] = [
    'description' => 'Download Apple News dependency PHP libraries.',
    'aliases' => ['an-dl'],
  ];

  return $items;

}

/**
 * Callback for the "applenews" command.
 */
function drush_applenews($operation, $module, $machine_name) {

  module_load_include('inc', 'applenews');
  module_load_include('inc', 'applenews', 'applenews.batch');

  $key = applenews_export_id($module, $machine_name);
  $info = applenews_exports()[$key];
  $export = applenews_get_export($module, $machine_name);

  if ($export->enabled) {

    switch ($operation) {

      // Run branch entity export to Apple News.
      case 'export' :
        $batch = applenews_batch_run_export($info, $export, 'export');
        $batch['progressive'] = FALSE;
        batch_set($batch);
        drush_log('Entities successfully exported to Apple News.', 'ok');
        break;

      // Download News Preview compatible files.
      case 'preview' :
        $batch = applenews_batch_run_export($info, $export, 'export-to-file');
        $batch['progressive'] = FALSE;
        batch_set($batch);
        drush_log('Apple News Native formatted entities successfully exported. The file archive file can be downloaded from `files/applenews/` directory.', 'ok');
        break;

    }

  }
  else {
    drush_log($info['name'] . ' export is disabled.', 'warning');
  }

}

/**
 * Callback for the "applenews-libraries" command.
 * Downloads remote ZIP archive, extracts the file and
 * create library specific directory.
 */
function drush_applenews_libraries() {
  module_load_include('inc', 'applenews');
  // Download and unzip PHP libraries.
  // @see applenews.inc file.
  applenews_autoinstall_libraries();
}
