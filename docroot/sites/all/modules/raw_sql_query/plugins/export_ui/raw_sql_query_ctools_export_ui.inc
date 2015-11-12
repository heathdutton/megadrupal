<?php
/**
* @file
* Define this Export UI plugin.
* https://drupal.org/node/928026
*/
$plugin = array(
  'schema' => 'raw_sql_query',  // As defined in hook_schema().
  'access' => 'administer content',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/development',
    'menu item' => 'raw_sql_query',
    'menu title' => 'Raw SQL query',
    'menu description' => 'Administer raw SQL queries.',
  ),
  'handler' => array(
    'class' => 'raw_sql_query_ui',
  ),
  // Define user interface texts.
  'title singular' => t('preset'),
  'title plural' => t('presets'),
  'title singular proper' => t('raw_sql_query preset'),
  'title plural proper' => t('raw_sql_query presets'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'raw_sql_query_ctools_export_ui_form',
    'submit' => 'raw_sql_query_ctools_export_ui_form_submit',
  ),
);

/**
* Define the preset add/edit form.
*/
function raw_sql_query_ctools_export_ui_form(&$form, &$form_state) {

  $preset = $form_state['item'];

  $form['menu_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu path'),
    '#description' => t('Enter menu path for display table'),
    '#default_value' => $preset->menu_path,
    '#required' => TRUE,
  );

  // Add Mymodule's configuration interface.
  $form['menu_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu title'),
    '#description' => t('Type menu title'),
    '#default_value' => $preset->menu_title,
    '#required' => TRUE,
  );

  $form['tablename'] = array(
    '#type' => 'textfield',
    '#title' => t('Table name'),
    '#description' => t('table name'),
    '#default_value' => $preset->tablename,
    '#required' => TRUE,
  );


  if (isset($preset->tablename) && db_table_exists(raw_sql_query_prefix_table($preset->tablename))) {
    $fields = raw_sql_query_show_columns(raw_sql_query_prefix_table($preset->tablename));
    foreach ($fields as $field) {
      $options[] = $field->Field;
    }

    $default_fields = explode(',', $preset->fields);

    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields'),
      '#description' => t('fields to filter using selector for exporting'),
      '#options' => drupal_map_assoc($options),
      '#default_value' => $default_fields,
      '#required' => TRUE,
    );
  }

  $form['sql_query'] = array(
    '#type' => 'textarea',
    '#title' => t('SQL query'),
    '#description' => t('The human readable name or description of this preset.'),
    '#default_value' => $preset->sql_query,
    '#required' => TRUE,
  );

}

/**
* Define the preset add/edit form.
*/
function raw_sql_query_ctools_export_ui_form_submit(&$form, &$form_state) {

  //drupal_flush_all_caches();

  $tablename = raw_sql_query_prefix_table($form_state['values']['tablename']);

  $old_tablename = isset($form_state['item']->tablename) ? raw_sql_query_prefix_table($form_state['item']->tablename) : '';

  $sql_query = $form_state['values']['sql_query'];

  raw_sql_query_drop_table($tablename);

  if (!is_null($form_state['item']->tablename)) {
    raw_sql_query_drop_table($old_tablename);
  }


  try {

    raw_sql_query_create_table($tablename, $sql_query);

  } catch (Exception $e) {

    drupal_set_message(t("There is a syntax error in your SQL query"), 'error');

  }

  if (isset($form_state['values']['fields'])) {
    $fields = implode(',', $form_state['values']['fields']);
    $form_state['values']['fields'] = $fields;
  }



}
/**
* Implementation of hook_ctools_plugin_api().
*
* Tell CTools that we support the default_mymodule_presets API.
*/
function raw_sql_query_ctools_plugin_api($owner, $api) {
  if ($owner == 'raw_sql_query' && $api == 'default_raw_sql_query_preset') {
    return array('version' => 1);
  }
}

/**
* Implementation of hook_default_mymodule_preset().
*
* Provide a couple of default presets.
*/

function raw_sql_query_default_raw_sql_query_preset() {
  $export = array();

  $example = new stdClass;
  $example->api_version = 1;
  $example->name = 'example';
  $example->menu_path = 'drinux';
  $example->menu_title = 'Drinux';
  $example->tablename = 'drinux';
  $example->fields = 'title,created';
  $example->sql_query = 'SELECT * FROM node LIMIT 0,10';
  $export['example'] = $example;

  return $export;
}

