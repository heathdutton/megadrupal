<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'access_token_export',
  'access' => 'administer site configuration',

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/people',
    'menu item' => 'access-token',
    'menu title' => 'Access Tokens',
    'menu description' => 'Administer access tokens.',
  ),

  // Define user interface texts.
  'title singular' => t('access token'),
  'title plural' => t('access tokens'),
  'title singular proper' => t('Access token'),
  'title plural proper' => t('Access tokens'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => array('CToolsAccessTokenExportUI', 'form'),
    'validate' => array('CToolsAccessTokenExportUI', 'validate'),
    'submit' => array('CToolsAccessTokenExportUI', 'submit'),
  ),
  'handler' => array(
    'class' => 'CToolsAccessTokenExportUI',
    'parent' => 'ctools_export_ui',
  ),
);
