<?php
/**
 * Features integration for the Cache External Files module.
 */

/**
 * Implements hook_features_export_options().
 */
function cache_external_files_features_export_options() {
  $options = array();
  $options['cache_external_files_data'] = 'Cache external files data';
  return $options;
}

/**
 * Implements hook_features_export().
 */
function cache_external_files_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['module'] = 'cache_external_files';

  foreach ($data as $component) {
    $export['features']['cache_external_files'][$component] = $component;
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 * Exports files from DB with 'url' as a key.
 */
function cache_external_files_features_export_render($module, $data, $export = NULL) {
  $query = db_select('cache_external_files', 'cef')
    ->fields('cef', array('url', 'timeout', 'enabled'));

  $result = $query->execute();

  $cache_external_files_data = '  $cache_external_files_data = ' . features_var_export($result->fetchAllAssoc('url'), '  ') . ';';

  $cache_external_files_data .= PHP_EOL . '  return $cache_external_files_data;';
  return array('cache_external_files_data_default' => $cache_external_files_data);
}

/**
 * Implements hook_features_revert().
 */
function cache_external_files_features_revert($module = NULL) {
  cache_external_files_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function cache_external_files_features_rebuild($module) {
  $default_files_data = module_invoke($module, 'cache_external_files_data_default');
  $query = db_select('cache_external_files', 'cef')
    ->fields('cef', array('fid', 'url', 'timeout', 'enabled'));
  $result = $query->execute();
  // $files_to_sync is used for further reference to delete files that are not in $default_files_data.
  $files_to_sync = $result->fetchAllAssoc('url');
  if (!empty($default_files_data) && is_array($default_files_data)) {
    foreach ($default_files_data as $file) {
      $url = $file['url'];

      // DO 'INSERT ... ON DUPLICATE KEY UPDATE ...'
      db_merge('cache_external_files')
        ->insertFields(array(
          'url' => $url,
          'timeout' => $file['timeout'],
          'enabled' => $file['enabled'],
        ))
        ->updateFields(array(
          'timeout' => $file['timeout'],
          'enabled' => $file['enabled'],
        ))
        ->key(array('url' => $url))
        ->execute();

      // File is synced, remove from $files_to_sync.
      if (!empty($files_to_sync[$url])) {
        unset($files_to_sync[$url]);
      }
    }
  }

  // Remove files that are not present in $default_files_data.
  if (!empty($files_to_sync)) {
    foreach ($files_to_sync as $item) {
      $fids_to_delete[] = $item->fid;
    }
    db_delete('cache_external_files')
      ->condition('fid', $fids_to_delete, 'IN')
      ->execute();
  }

  // Update newly created or expired files.
  cache_external_files_cron();
}
