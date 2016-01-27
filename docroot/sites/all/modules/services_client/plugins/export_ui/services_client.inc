<?php

/**
* Define this Export UI plugin.
*/
$plugin = array(
  // The name of the table as found in the schema in hook_install. This
  // must be an exportable type with the 'export' section defined.
  'schema' => 'services_client_connection_event',
  // The access permission to use. If not provided it will default to
  // 'administer site configuration'
  'access' => 'administer services client',
  // You can actually define large chunks of the menu system here. Nothing
  // is required here. If you leave out the values, the prefix will default
  // to admin/structure and the item will default to the plugin name.
  'menu' => array(
    'menu item' => 'services_client',
    // Title of the top level menu. Note this should not be translated,
    // as the menu system will translate it.
    'menu title' => 'Services Client',
    // Description of the top level menu, which is usually needed for
    // menu items in an administration list. Will be translated
    // by the menu system.
    'menu description' => 'Administer Services Client Connection Events.',
    // Add services specific own menu callbacks.
    'items' => array(
      'configure' => array(
        'path' => 'list/%ctools_export_ui/configure',
        'title' => 'Configure',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('services_client', 'configure', 4),
        'load arguments' => array('services_client'),
        'access arguments' => array('administer services client'),
        'type' => MENU_LOCAL_TASK,
      ),
      'break-lock' => array(
        'path' => 'list/%ctools_export_ui/break-lock',
        'title' => 'Configure',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('services_client', 'break_lock', 4),
        'load arguments' => array('services_client'),
        'access arguments' => array('administer services client'),
        'type' => MENU_VISIBLE_IN_BREADCRUMB,
      ),
      'add_plugin' => array(
        'path' => 'list/%ctools_export_ui/add_plugin/%',
        'title' => 'Configure',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('services_client', 'add_plugin', 4, 6),
        'load arguments' => array('services_client'),
        'access arguments' => array('administer services client'),
        'type' => MENU_CALLBACK,
      ),
      'configure_plugin' => array(
        'path' => 'list/%ctools_export_ui/plugin/%/%/edit',
        'title' => 'Configure',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('services_client', 'configure_plugin', 4, 6, 7),
        'load arguments' => array('services_client'),
        'access arguments' => array('administer services client'),
        'type' => MENU_CALLBACK,
      ),
      'remove_plugin' => array(
        'path' => 'list/%ctools_export_ui/plugin/%/%/remove',
        'title' => 'Configure',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('services_client', 'remove_plugin', 4, 6, 7),
        'load arguments' => array('services_client'),
        'access arguments' => array('administer services client'),
        'type' => MENU_CALLBACK,
      ),
    ),
  ),

  // These are required to provide proper strings for referring to the
  // actual type of exportable. "proper" means it will appear at the
  // beginning of a sentence.
  'title singular' => t('event'),
  'title plural' => t('events'),
  'title singular proper' => t('Services Client Event'),
  'title plural proper' => t('Services Client events'),

  // This will provide you with a form for editing the properties on your
  // exportable, with validate and submit handler.
  //
  // The item being edited will be in $form_state['item'].
  //
  // The submit handler is only responsible for moving data from
  // $form_state['values'] to $form_state['item'].
  //
  // All callbacks will accept &$form and &$form_state as arguments.

  // Add our custom operations.
  'allowed operations' => array(
    'configure' => array('title' => t('Configure')),
  ),
  'form' => array(
    'settings' => 'services_client_ctools_export_ui_form',
    'submit' => 'services_client_ctools_export_ui_form_submit',
  ),
  'handler' => array(
    'class' => 'services_client_ui',
    'parent' => 'ctools_export_ui',
  ),
);

