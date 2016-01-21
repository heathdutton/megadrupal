<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'osf_configure_endpoints',  // As defined in hook_schema().
  'access' => 'administer osf config',  // Define a permission users must have to access these pages

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/osf/networks_and_dataset',
    'menu item' => 'endpoints',
    'menu title' => t('OSF API Endpoints'),
    'menu description' => t('Administer OSF API Endpoints.'),
  ),

  // Define user interface texts.
  'title singular' => t('OSF for Drupal API endpoint'),
  'title plural' => t('OSF for Drupal API endpoints'),
  'title singular proper' => t('OSF API Endpoint'),
  'title plural proper' => t('OSF API Endpoints'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'osf_configure_endpoint_form',
    'validate' => 'osf_configure_endpoint_form_validate',
    'submit' => 'osf_configure_endpoint_form_submit',
  ),
  'handler' => array(
     'class' => 'ctools_export_ui_osf_configure_endpoints',
     'parent' => 'ctools_export_ui',
  ),
);