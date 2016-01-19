<?php

/**
 * @file
 * Intergrates Entity bundle Redirect with Features.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

/**
 * Implements hook_features_export_options().
 */
function entity_bundle_redirect_features_export_options() {

  // Used to avoid ventral review warnings.
  $t = get_t();

  $sql = db_select('entity_bundle_redirect', 'b')
    ->fields('b')
    ->execute();
  $options = array();
  $languages = language_list();
  while ($row = $sql->fetchAssoc()) {
    $key = $row['entity_type'] . ':' . $row['entity_bundle'] . ':' . $row['language'];
    $name = $row['entity_type'] . ': ' . $row['entity_bundle'] . ' (' . $t($languages[$row['language']]->name) . ')';
    $options[$key] = $name;
  }
  return $options;

}

/**
 * Implements hook_features_export().
 */
function entity_bundle_redirect_features_export($data, &$export, $module_name = '') {

  $export['dependencies']['features'] = 'features';
  $export['dependencies']['entity_bundle_redirect'] = 'entity_bundle_redirect';
  if (module_exists('token')) {
    $export['dependencies']['token'] = 'token';
  }
  $sql = db_select('entity_bundle_redirect', 'ebr')
    ->fields('ebr')
    ->execute();
  while ($row = $sql->fetchAssoc()) {
    $key = $row['entity_type'] . ':' . $row['entity_bundle'] . ':' . $row['language'];
    if (in_array($key, $data)) {
      $export['features']['entity_bundle_redirect'][$key] = $key;
    }
  }
  return $export;

}

/**
 * Implements hook_features_export_render().
 */
function entity_bundle_redirect_features_export_render($module, $data) {

  $code[] = '  $entity_bundle_redirect = array();';
  $code[] = '';
  foreach ($data as $delta) {
    $data_array = explode(':', $delta);
    $redirection = entity_bundle_redirect_load_stored(
      $data_array[0],
      $data_array[1],
      $data_array[2]
    );
    if ($redirection) {
      $redirection_export = features_var_export($redirection, '  ');
      $code[] = '  // Exported entity bundle redirection: ' . $delta;
      $code[] = '  $entity_bundle_redirect[\'' . $delta . '\'] = ' . $redirection_export . ';';
      $code[] = '';
    }
  }
  $code[] = '  return $entity_bundle_redirect;';
  $code = implode("\n", $code);
  return array('entity_bundle_redirect_default_config' => $code);

}

/**
 * Implements hook_features_export_revert().
 */
function entity_bundle_redirect_features_revert($module) {

  module_load_include('inc', 'features', 'features.export');
  foreach (features_get_default('entity_bundle_redirect', $module) as $key => $url) {
    $data_array = explode(':', $key);
    entity_bundle_redirect_save_url(
      $data_array[0],
      $data_array[1],
      $url,
      $data_array[2]
    );
  }
  cache_clear_all('*', 'cache_entity_bundle_redirect', TRUE);

}

/**
 * Implements hook_features_export_rebuild().
 */
function entity_bundle_redirect_features_export_rebuild($module) {

  entity_bundle_redirect_features_revert($module);

}
