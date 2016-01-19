<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'osf_searchprofiles',  // As defined in hook_schema().
  'access' => 'administer search profiles',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/search',
    'menu item' => 'profiles',
    'menu title' => 'Search Profiles',
    'menu description' => 'Administer search profiles.',
  ),

  // Define user interface texts.
  'title singular' => t('search profile'),
  'title plural' => t('search profiles'),
  'title singular proper' => t('OSF search profile'),
  'title plural proper' => t('OSF search profiles'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'osf_searchprofiles_ctools_export_ui_form',
    'validate' => 'osf_searchprofiles_form_validate',
  ),
);

/**
 * Define the sprofile add/edit form.
 */
function osf_searchprofiles_ctools_export_ui_form(&$form, &$form_state) {
  $sprofile = $form_state['item'];

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('The human readable name or description of this sprofile.'),
    '#default_value' => $sprofile->description,
    '#required' => TRUE
  );                                                 

  // If we're adding a new SP, allow to paste JSON code.
  $form['import_settings'] = array(
    '#type' => 'textarea',
    '#title' => t('Settings'),
    '#description' => t('Paste the JSON-serialized settings copied from the Query Builder'),
    '#default_value' => !empty($sprofile->settings) ? json_encode($sprofile->settings, JSON_PRETTY_PRINT) : '',
    '#cols' => 60,
    '#rows' => 9,
  );
}    

/**
 * Validate callback for the sprofile form
 */
function osf_searchprofiles_form_validate($form, &$form_state) {  
  // Convert the JSON settings to an object and save it in settings.
  if (isset($form_state['values']['import_settings'])) {
    $json = $form_state['values']['import_settings'];         
    $decoded = drupal_json_decode($json);  
    if ($decoded) {
      $form_state['values']['settings'] = $decoded;
    }
  }
  if (!empty($form_state['values']['name']) && strpos($form_state['values']['name'], '/') !== FALSE) {
    form_set_error('name', t('The name cannot contain the "/" character.'));
  }
}


