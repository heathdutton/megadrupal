<?php
/**
 * @file
 *
 * Provides drush extensions for Arte OPA
 */

/**
 * Implements hook_drush_command().
 */
function arte_opa_broadcasts_drush_command() {
  $items = array();

  $items['opa-broadcasts-sync'] = array(
    'description' => 'Synchronizes upcoming programs from OPA in both French and German',
    'aliases' => array('obs'),
    'options' => array(
      'debug-opa' => 'Output debugging information about the OPA requests issued during synchronization.',
      'caseCode' => 'Limit the synchronization of videos to the specified magazine case codes, e.g. --caseCode=310 (Karambolage).',
    ),
    'examples' => array(
      'obs' => 'Updates programs from OPA in the next 10 days, for both French and German programmation',
      'obs --caseCode=310,315' => 'Updates programs for only "Karambolage" (310) and "ARTE Junior" (315) videos.',
    ),
  );

  return $items;
}

/**
 * Command callback: run OPA broadcasts synchronization
 */
function drush_arte_opa_broadcasts_opa_broadcasts_sync($args = NULL) {
  // Query OPA for upcoming broadcasts
  $debug = drush_get_option('debug-opa', FALSE);
  $caseCodes = drush_get_option('caseCode', array());
  drush_log(dt('Starting OPA broadcasts synchronization...'), 'status');
  _drush_arte_opa_broadcasts_opa_broadcasts_sync($caseCodes, $debug);
  drush_log(dt('OPA broadcasts synchronization finished.'), 'success');
}

/**
 * Synchronize OPA data and create or update video pages.
 *
 * @param array $caseCodes the APIOS case codes to filter the OPA request.
 * @param  boolean $debug wether to output debugging information.
 */
function _drush_arte_opa_broadcasts_opa_broadcasts_sync($caseCodes = array(), $debug = FALSE) {
  // Create the OPA connector
  module_load_include('inc', 'arte_opa', 'includes/opa');
  module_load_include('inc', 'arte_opa', 'includes/opa_reader');
  module_load_include('inc', 'arte_opa_broadcasts', 'includes/opa_broadcasts_reader');
  $opa = arte_opa_get_api();
  if ($debug) {
    $opa->setDebug();
  }

  // Query OPA for upcoming broadcasts and merge the multilingual datasets by program ID
  $languages = arte_opa_languages();
  $broadcasts = array();
  foreach ($languages as $lang) {
    // Fetch the broadcasts with program data
    $params = array(
      'language' => $lang,
      'case.code' => implode(',', $caseCodes),
      'sort' => 'AIRDATE_ASC',
      // Include program data
      'include' => 'programs',
    );

    drupal_alter('arte_opa_broadcasts_sync_params', $params);

    $reader = new OPABroadcastsReader($opa, $params);

    foreach ($reader as $data) {
      $broadcasts[$data['programId']][$lang] = $data;
    }
  }

  // Now let other modules do something with the broadcasts data
  module_invoke_all('arte_opa_broadcasts_sync', $broadcasts);
}
