<?php
/**
 * @file
 * Contains the exportable UI for the Socrata module's sources
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  // As defined in hook_schema().
  'schema' => 'socrata_sources',
  // Define a permission users must have to access these pages.
  'access' => 'administer socrata',

  // Define the menu item.
  'menu' => array(
    'menu item' => 'socrata',
    'menu title' => 'Socrata',
    'menu description' => 'Administer Socrata sources',
  ),

  // Define user interface texts.
  'title singular' => t('source'),
  'title plural' => t('sources'),
  'title singular proper' => t('Socrata source'),
  'title plural proper' => t('Socrata sources'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => '_socrata_ctools_export_ui_form',
    'validate' => '_socrata_ctools_export_ui_form_validate',
    'submit' => '_socrata_ctools_export_ui_form_submit',
  ),
);

/**
 * Define the source add/edit form.
 */
function _socrata_ctools_export_ui_form(&$form, &$form_state) {
  $source = $form_state['item'];

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('The human readable name or description of this source'),
    '#default_value' => $source->description,
    '#required' => TRUE,
  );

  $form['endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint URL'),
    '#description' => t('The endpoint URL for the dataset'),
    '#default_value' => $source->endpoint,
    '#required' => TRUE,
  );

  $form['app_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Application token'),
    '#description' => t('The application token for the dataset (optional, but recommended if you have it)'),
    '#default_value' => $source->app_token,
    '#required' => FALSE,
  );

}

/**
 * Handle validation of the source add/edit form.
 */
function _socrata_ctools_export_ui_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];

  // Validate endpoint URL for dataset.
  if (!valid_url($values['endpoint'], TRUE)) {
    form_set_error('endpoint', t('The URL is not properly formed'));
  }
}

/**
 * Handle submission of the source add/edit form.
 */
function _socrata_ctools_export_ui_form_submit(&$form, &$form_state) {
  // Clear out Views table definition caches so rebuilds will pick up
  // added/removed/edited sources.
  if (module_exists('views')) {
    views_invalidate_cache();
  }
}
