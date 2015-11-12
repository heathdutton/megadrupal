<?php

/**
* Define this Export UI plugin.
*/
$plugin = array(
  'schema' => 'party_dashboard_context',  // As defined in hook_schema().
  'access' => 'administer party dashboard contexts',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/party',
    'menu item' => 'dashboard-context',
    'menu type' => MENU_DEFAULT_LOCAL_TASK,
    'menu title' => 'Dashboard Context',
    'menu description' => 'Administer Party Dashboard contexts.',
  ),

  // Define user interface texts.
  'title singular' => t('context'),
  'title plural' => t('contexts'),
  'title singular proper' => t('Dashboard Context'),
  'title plural proper' => t('Dashboard Contexts'),

  'handler' => array(
    'class' => 'party_dashboard_context_ui',
    'parent' => 'ctools_export_ui',
  ),
);
