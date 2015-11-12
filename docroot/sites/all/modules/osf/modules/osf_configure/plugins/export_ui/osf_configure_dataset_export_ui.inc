<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'osf_configure_datasets',  // As defined in hook_schema().
  'access' => 'administer osf config',  // Define a permission users must have to access these pages

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/osf/networks_and_dataset',
    'menu item' => 'datasets',
    'menu title' => t('OSF Datasets'),
    'menu description' => t('Configure OSF Datasets.'),
  ),

  // Define user interface texts.
  'title singular' => t('OSF for Drupal dataset'),
  'title plural' => t('OSF for Drupal datasets'),
  'title singular proper' => t('OSF Dataset'),
  'title plural proper' => t('OSF Datasets'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'osf_configure_dataset_form',
    'validate' => 'osf_configure_dataset_form_validate',
    'submit' => 'osf_configure_dataset_form_submit',
  ),
  'handler' => array(
     'class' => 'ctools_export_ui_osf_configure_datasets',
     'parent' => 'ctools_export_ui',
  ),
);