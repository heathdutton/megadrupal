<?php
/**
 * @file
 * Ctool access plugin
 *
 * Checks if a search server configured in a given search index supports a given
 * feature.
 */

$plugin = array(
  'title' => t('Search API Server Support'),
  'description' => t('Controls access by the supported features of the search server.'),
  'callback' => 'sharedcontent_ui_search_api_server_support_access',
  'default' => array(
    'index' => NULL,
    'feature' => NULL,
  ),
  'settings form' => 'sharedcontent_ui_search_api_server_support_settings',
  'summary' => 'sharedcontent_ui_search_api_server_support_summary',
);

/**
 * Checks if the search server supports the given features.
 */
function sharedcontent_ui_search_api_server_support_access($conf, $context) {
  // If for some reason that $conf isn't set, we just want to be sure.
  if (empty($conf) || empty($conf['index']) || empty($conf['feature'])) {
    return FALSE;
  }

  $index = entity_load('search_api_index', FALSE, array(
    'machine_name' => $conf['index'],
  ));

  if (!$index) {
    return FALSE;
  }

  $server = entity_load('search_api_server', FALSE, array(
    'machine_name' => reset($index)->server,
  ));

  return $server ? reset($server)->supportsFeature($conf['feature']) : FALSE;
}

/**
 * Settings form for the 'search_api_server_support' access plugin.
 */
function sharedcontent_ui_search_api_server_support_settings($form, &$form_state, $conf) {

  $form['settings']['index'] = array(
    '#type' => 'select',
    '#title' => t('Search Index'),
    '#options' => array('' => t('< No index >')),
    '#description' => t('The search index the server will be fetched from.'),
    '#default_value' => $conf['index'],
  );

  if ($indexes = entity_load('search_api_index')) {
    foreach ($indexes as $index) {
      $form['settings']['index']['#options'][$index->machine_name] = t($index->name);
    }
  }

  $form['settings']['feature'] = array(
    '#type' => 'textfield',
    '#title' => t('Feature'),
    '#description' => t('The feature the search server must support.'),
    '#default_value' => $conf['feature'],
  );
  return $form;
}

/**
 * Provide a summary description based upon the entered feature.
 */
function sharedcontent_ui_search_api_server_support_summary($conf, $context) {
  return t('The search server supports @feature', array('@feature' => $conf['feature']));
}
