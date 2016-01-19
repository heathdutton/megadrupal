<?php

/**
 * @file
 * Defines a Google Search Collections preset add/edit form.
 */

$plugin = array(
  'schema' => 'google_search_collections',
  'access' => 'administer google search collections',
  'menu' => array(
    'menu item' => 'google-search-collections',
    'menu title' => 'Google search collections',
    'menu description' => 'Administer google search collections.',
  ),
  'title singular' => t('collection'),
  'title plural' => t('collections'),
  'title singular proper' => t('Google search collection'),
  'title plural proper' => t('Google search collections'),
  'form' => array(
    'settings' => 'google_search_collections_ctools_export_ui_form',
  ),
);


/**
 * Define the search collection add/edit form.
 */
function google_search_collections_ctools_export_ui_form(&$form, &$form_state) {
  $preset = $form_state['item'];

  // Create a vertical tab set.
  $form['tabs'] = array('#type' => 'vertical_tabs');

  // Modify the "info" key to be a fieldset.
  $form['info']['#title'] = t('Collection details');
  $form['info']['#type'] = 'fieldset';
  $form['info']['#group'] = 'tabs';

  // Provide a fieldset to contain all "data" customization attributes.
  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customizations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'tabs',
    '#tree' => TRUE,
  );

  // Provide more useful description text for the path field.
  $form['info']['path']['#description'] = t('The unique URL path part for this collection. Once saved, this cannot be changed.');

  // Search collection title.
  $form['info']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection title'),
    '#description' => t('The title for this search collection, displayed to end-users.'),
    '#default_value' => $preset->title,
    '#required' => TRUE,
  );

  // Search collection description.
  $form['info']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Enter notes about this search collection for yourself or future administrators.'),
    '#default_value' => $preset->description,
  );

  // Module.
  $form['data']['module'] = array(
    '#type' => 'select',
    '#title' => t('Module'),
    '#description' => t('The module that powers this search collection.'),
    '#default_value' => isset($preset->data['module']) ? $preset->data['module'] : '',
    '#options' => array(),
    '#required' => TRUE,
  );

  if (module_exists('gss')) {
    $form['data']['module']['#options']['gss'] = t('Google Site Search');
  }
  if (module_exists('google_appliance')) {
    $form['data']['module']['#options']['google_appliance'] = t('Google Search Appliance');
  }

  // Collection block AJAX enabled
  $form['data']['ajax_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable AJAX on search collection block'),
    '#description' => t('Adds AJAX functionality to the Google Search Collections block corresponding to this collection.'),
    '#default_value' => isset($preset->data['ajax_block']) ? $preset->data['ajax_block'] : FALSE,
  );

  // Number of results to return in AJAX mode.
  $form['data']['ajax_num_results'] = array(
    '#type' => 'select',
    '#title' => t('AJAX result count'),
    '#description' => t('Limits the number of results displayed when returned via AJAX.'),
    '#default_value' => isset($preset->data['ajax_num_results']) ? $preset->data['ajax_num_results'] : 3,
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
    '#states' => array(
      'visible' => array(
        ':input[name="data[ajax_block]"]' => array(
          'checked' => TRUE,
        ),
      ),
    )
  );

  // Collection page enabled/disabled toggle.
  $form['data']['disable_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable search result page'),
    '#description' => t('Useful when you only wish to expose this collection as a block'),
    '#default_value' => isset($preset->data['disable_page']) ? $preset->data['disable_page'] : FALSE,
  );

  // Collection weight.
  $form['data']['weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#description' => t('Determines the display order of this collection relative to others.'),
    '#default_value' => isset($preset->data['weight']) ? $preset->data['weight'] : 0,
    '#options' => drupal_map_assoc(array(-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
    '#states' => array(
      'visible' => array(
        ':input[name="data[disable_page]"]' => array(
          'checked' => FALSE,
        ),
      ),
    )
  );

  // Query terms.
  $form['data']['queryterms'] = array(
    '#type' => 'textfield',
    '#title' => t('Query terms'),
    '#description' => t('Special query terms that are appended to all searches performed within this collection.'),
    '#default_value' => isset($preset->data['queryterms']) ? $preset->data['queryterms'] : '',
    '#maxlength' => NULL,
  );

  // Google Appliance: Collection
  $form['data']['collection'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection'),
    '#description' => t('Use this to specify a GSA collection that this search collection should target.'),
    '#default_value' => isset($preset->data['collection']) ? $preset->data['collection'] : '',
    '#states' => _google_search_collections_by_module_state('google_appliance'),
  );

  // Google Appliance: Frontend
  $form['data']['frontend'] = array(
    '#type' => 'textfield',
    '#title' => t('Frontend'),
    '#description' => t('Use this to specify a GSA frontend that this search collection should target.'),
    '#default_value' => isset($preset->data['frontend']) ? $preset->data['frontend'] : '',
    '#states' => _google_search_collections_by_module_state('google_appliance'),
  );

  // Google Appliance: Required meta tags.
  $form['data']['requiredfields'] = array(
    '#type' => 'textfield',
    '#title' => t('Required fields'),
    '#description' => t('Use this to filter down to results that contain specific meta tag attribute/value pairs.'),
    '#default_value' => isset($preset->data['requiredfields']) ? $preset->data['requiredfields'] : '',
    '#states' => _google_search_collections_by_module_state('google_appliance'),
    '#maxlength' => NULL,
  );
}

/**
 * Returns a Drupal Form API States array given the module dependency.
 *
 * @param string
 *   The name of the expected module.
 *
 * @return array
 */
function _google_search_collections_by_module_state($module) {
  return array(
    'visible' => array(
      ':input[name="data[module]"]' => array(
        'value' => $module,
      ),
    ),
  );
}
