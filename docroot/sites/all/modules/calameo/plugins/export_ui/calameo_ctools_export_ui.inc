<?php
/**
 * @file
 *
 * Contains the exportable UI for the Socrata module's sources
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'calameo_accounts',  // As defined in hook_schema().
  'access' => 'administer calameo accounts',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu item' => 'calameo',
    'menu title' => 'Calameo',
    'menu description' => 'Administer Calameo accounts',
  ),

  // Define user interface texts.
  'title singular' => t('account'),
  'title plural' => t('accounts'),
  'title singular proper' => t('Calameo account'),
  'title plural proper' => t('Calameo accounts'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => '_calameo_ctools_export_ui_form',
    'validate' => '_calameo_ctools_export_ui_form_validate',
    'submit' => '_calameo_ctools_export_ui_form_submit',
  ),
  'handler' => array(
    'class' => 'calameo_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);

/**
 * Define the source add/edit form
 */
function _calameo_ctools_export_ui_form(&$form, &$form_state) {
  $source = $form_state['item'];

  $form['human_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Human-readable name'),
    '#description' => t('The human readable name for this account'),
    '#default_value' => $source->human_name,
    '#required' => TRUE,
  );

  $form['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint URL'),
    '#description' => t('The endpoint URL for the API'),
    '#default_value' => !empty($source->endpoint) ? $source->endpoint : 'http://api.calameo.com/1.0',
    '#required' => TRUE,
  );

  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('The API key provided by Calameo.'),
    '#default_value' => $source->api_key,
    '#required' => TRUE,
  );

  $form['api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('API secret'),
    '#description' => t('The API secret provided by Calameo'),
    '#default_value' => $source->api_secret,
    '#required' => TRUE,
  );
}

/**
 * Handle validation of the source add/edit form
 */
function _calameo_ctools_export_ui_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];

  // Validate endpoint URL for dataset
  if (!valid_url($values['endpoint'], TRUE)) {
    form_set_error('endpoint', t('The URL is not properly formed'));
  }
}

/**
 * Handle submission of the source add/edit form
 */
function _calameo_ctools_export_ui_form_submit(&$form, &$form_state) {
}

