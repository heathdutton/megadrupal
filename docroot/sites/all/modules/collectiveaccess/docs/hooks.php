<?php

// $Id$

/**
 * @file
 * Documentation regarding hooks provided by the CollectiveAccess module
 */

/**
 * hook_collectiveaccess_connector().
 *
 * This hook allows other modules to register other connector classes
 * to connect to the CollectiveAccess API.
 * These classes should implement interface ICollectiveAccessConnector.
 */
function modulename_collectiveaccess_connector() {
  return array(
    'MyConnectorClassName' => array( // key should be class name
      'name' => 'My Connector', // define a human-readable name for your connector
      'module' => 'modulename',
      'file' => 'myconnector.inc', // specify in which file the class can be found
      // path where file can be found (optional - defaults to current module path)
      'file path' => drupal_get_path('module', 'modulename') . '/includes',
    ),
  );
}

/**
 * hook_collectiveaccess_connectors_alter().
 *
 * This hooks allows you to alter the settings array that is being fetched by collectiveaccess_get_connectors()
 */
function modulename_collectiveaccess_connectors_alter(&$connectors) {
  // example code from collectiveaccess_ui module:
  foreach ($connectors as $key => &$connector) {
    if ($key == 'CollectiveAccessSOAPConnector' || $key == 'CollectiveAccessRESTConnector') {
      $connector['settings_form_callback'] = 'collectiveaccess_ui_connector_settings_form';
    }
  }
}