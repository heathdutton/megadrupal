<?php

/**
 * @file
 * This file contains the export ui required functions
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'adlibapi_databases',
  'access' => 'administer adlibapi databases',
  'menu' => array(
    'menu item' => 'adlibapi',
    'menu title' => 'Adlib API',
    'menu description' => 'Administer Adlibapi database presets.',
  ),
  'title singular' => t('preset'),
  'title plural' => t('presets'),
  'title singular proper' => t('Adlibapi preset'),
  'title plural proper' => t('Adlibapi presets'),
  'form' => array(
    'settings' => 'adlibapi_ctools_export_ui_form',
    'validate' => 'adlibapi_ctools_export_ui_form_validate',
    'submit' => 'adlibapi_ctools_export_ui_form_submit',
  ),
  'redirect' => array(
    'add' => current_path(),
  ),
);

/**
 * Define the preset add/edit form.
 */
function adlibapi_ctools_export_ui_form(&$form, &$form_state) {
  $preset = $form_state['item'];
  if (isset($preset->config)) {
    adlibapi_deserialize_config($preset);
  }
  // Get a list of databases if the url is given.
  $database_options = array();
  $form_state['db_found'] = FALSE;
  if (isset($preset->url)) {
    $databases = adlibapi_get_available_databases_at_url($preset->url);
    if (count($databases) > 0) {
      $form_state['db_found'] = TRUE;
      $database_options[0] = t('Select database');
      foreach ($databases as $db) {
        $database_options[$db['database']] = $db['database'];
      }
    }
    else {
      if (isset($preset->database)) {
        $database_options[$preset->database] = $preset->database;
      }
    }
  }
  if (count($database_options) == 0) {
    $database_options[0] = t('No databases found on url');
  }
  /*
   * Get the version of the database.
   *
   * If the version is lower than the recomended version,
   * show database textfield instead of select.
   */
  $show_select = TRUE;
  $servercontacted = FALSE;
  if (isset($preset->url)) {
    $version = adlibapi_api_get_version($preset);
    if (count($version) == 0) {
      drupal_set_message(t('Unable to connect to OPAC server.'), 'error');
    }
    else {
      // Show error if version is less than minimum version.
      if (version_compare($version['Version'], ADLIBAPI_MINIMUM_WWWOPAC_VERSION, "<")) {
        $show_select = FALSE;
        drupal_set_message(t('The api version of wwwopac is lower than the recomended version. Recomended is %minimum or higher, the current version is %current', array(
          '%minimum' => ADLIBAPI_MINIMUM_WWWOPAC_VERSION,
          '%current' => $version['Version'],
        )), 'warning');
      }
      else {
        drupal_set_message(t('The OPAC server was contacted.'));
      }
      $servercontacted = TRUE;
    }
  }
  // Don't show select, if no databases are retrieved.
  // It is known that the adblibapi is not entirely stable,
  // so the list is not always retrieved.
  if (!isset($databases) || count($databases) == 0) {
    $show_select = FALSE;
  }
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('The human readable name or description of this preset.'),
    '#default_value' => $preset->description,
    '#required' => TRUE,
  );
  $form['config'] = array(
    '#type' => 'fieldset',
    '#description' => t('Database connection setting'),
    '#tree' => TRUE,
  );

  $form['config']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#description' => t('The url of the Adlib server.'),
    '#default_value' => isset($preset->url) ? $preset->url : '',
    '#required' => TRUE,
    '#weight' => 1,
  );
  if ($show_select) {
    $form['config']['database'] = array(
      '#type' => 'select',
      '#title' => t('Database'),
      '#description' => t('The database on this server'),
      '#options' => $database_options,
      '#default_value' => (isset($preset->database)) ? $preset->database : 0,
      '#weight' => 2,
    );
  }
  else {
    $form['config']['database'] = array(
      '#type' => 'textfield',
      '#title' => t('Database'),
      '#description' => t('The database on this url.'),
      '#default_value' => (isset($preset->database)) ? $preset->database : 'collect.inf',
      '#weight' => 2,
    );
  }
  $form['config']['imageserver'] = array(
    '#type' => 'textfield',
    '#title' => t('Image server'),
    '#description' => t('The image server name on this url'),
    '#default_value' => (isset($preset->imageserver)) ? $preset->imageserver : 'adlibimages',
    '#weight' => 5,
  );
  if ($servercontacted) {
    $fieldinfo = adlibapi_api_get_fieldList($preset);
    $database_info = theme('adlibapi_render_database_info', array(
      'versioninfo' => $version,
      'fieldinfo' => $fieldinfo,
    ));
    $form['config']['info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Database version and field information'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 3,
    );
    $form['config']['info']['info'] = array(
      '#type' => 'markup',
      '#markup' => $database_info,
    );
  }
}

/**
 * Validate function.
 */
function adlibapi_ctools_export_ui_form_validate($form, &$form_state) {
  // Validate url.
  $valid = valid_url($form_state['values']['config']['url'], TRUE);
  if (!$valid) {
    form_set_error('config][url', t('The url "%url" is not valid', array('%url' => $form_state['values']['config']['url'])));
  }
  // If databases are found on the url, selecting a database is required.
  if ($form_state['db_found'] && $form_state['values']['config']['database'] == '0') {
    form_set_error('config][database', t('You must select a database'));
  }
}

/**
 * Submit function.
 */
function adlibapi_ctools_export_ui_form_submit($form, &$form_state) {
  $form_state['values']['config'] = serialize($form_state['values']['config']);
  // If the preset is just made, redirect to the edit page to select a database.
  $obj = $form_state['object'];
  if (isset($form_state['item']->name)) {
    $name = $form_state['item']->name;
  }
  else {
    $name = $form_state['values']['name'];
  }
  $formurl = str_replace('%ctools_export_ui', $name, $obj->plugin['menu']['items']['edit']['path']);
  $obj->plugin['redirect']['add'] = $obj->plugin['menu']['menu prefix'] . '/' . $obj->plugin['menu']['menu item'] . '/' . $formurl;
  $obj->plugin['redirect']['edit'] = $obj->plugin['redirect']['add'];
}
