<?php

/**
 * @file
 * Ctools plugin related functions
 */

/**
 * Implements hook_ctools_plugin_type().
 */
function services_client_ctools_plugin_type() {
  return array(
    'condition' => array(
      'cache' => FALSE,
      'use hooks' => TRUE,
      'classes' => array('handler'),
    ),
    'mapping' => array(
      'cache' => FALSE,
      'use hooks' => TRUE,
      'classes' => array('handler'),
    ),
    'event_handler' => array(
      'cache' => FALSE,
      'use hooks' => TRUE,
      'classes' => array('handler'),
    ),
  );
}

/**
 * Implements hook_ctools_plugin_api().
 */
function services_client_ctools_plugin_api($owner, $api) {
  if ($owner == 'services_client' && in_array($api, array('mapping', 'condition', 'event_handler'))) {
    return array('version' => 1);
  }
}

/**
 * Implements hook_ctools_plugin_directory().
 */
function services_client_ctools_plugin_directory($owner, $plugin_type) {
  if ($owner == 'ctools' && $plugin_type == 'export_ui') {
    return 'plugins/' . $plugin_type;
  }
}

/**
 * Get list of all plugins
 *
 * @param string $type
 *   Type of required plugins
 *
 * @param bool $select_box
 *   Whether should be returned back as options for select box in format
 *   PluginName => Human Readable
 *
 * @param callable $filter
 *   Function that can be used in array_filter
 *
 * @return array
 *   List of plugins.
 */
function services_client_get_plugins($type, $select_box = FALSE, $filter = NULL) {
  ctools_include('plugins');
  $plugins = ctools_get_plugins('services_client', $type);

  // Let other modules alter list of plugins
  drupal_alter('services_client_plugins', $plugins, $type);

  if (is_callable($filter)) {
    $plugins = array_filter($plugins, $filter);
  }

  if ($select_box) {
    $plugins = array_map(function ($item) {
      return $item['name'];
    }, $plugins);
  }

  return $plugins;
}

/**
 * List available mapping plugins.
 *
 * @return array
 *   Plugins definition.
 */
function services_client_services_client_mapping() {
  $path = drupal_get_path('module', 'services_client') . '/include';

  $info = array();

  $info['ServicesClientPropertyReader'] = array(
    'name' => t('Property'),
    'description' => 'Read property from entity',
    'handler' => array(
      'parent' => 'ServicesClientMapperPlugin',
      'class' => 'ServicesClientPropertyReader',
      'file' => 'mapping.inc',
      'path' => $path,
    ),
    'type' => 'reader',
  );

  $info['ServicesClientFieldReader'] = array(
    'name' => t('Field'),
    'description' => 'Reads field value(s) from entity',
    'handler' => array(
      'parent' => 'ServicesClientMapperPlugin',
      'class' => 'ServicesClientFieldReader',
      'file' => 'mapping.inc',
      'path' => $path,
    ),
    'type' => 'reader',
  );

  $info['ServicesClientPropertyFormatter'] = array(
    'name' => t('Property'),
    'description' => 'Create property on entity',
    'handler' => array(
      'parent' => 'ServicesClientMapperPlugin',
      'class' => 'ServicesClientPropertyFormatter',
      'file' => 'mapping.inc',
      'path' => $path,
    ),
    'type' => 'formatter',
  );

  $info['ServicesClientFieldFormatter'] = array(
    'name' => t('Field'),
    'description' => 'Create field on entity',
    'handler' => array(
      'parent' => 'ServicesClientMapperPlugin',
      'class' => 'ServicesClientFieldFormatter',
      'file' => 'mapping.inc',
      'path' => $path,
    ),
    'type' => 'formatter',
  );

  $info['ServicesClientFieldD6Formatter'] = array(
    'name' => t('Field (D6, D5)'),
    'description' => 'Create field on entity',
    'handler' => array(
      'parent' => 'ServicesClientFieldFormatter',
      'class' => 'ServicesClientFieldD6Formatter',
      'file' => 'mapping.inc',
      'path' => $path,
    ),
    'type' => 'formatter',
  );

  return $info;
}

/**
 * List availalable condition plugins.
 *
 * @return array
 *   Plugins definition.
 */
function services_client_services_client_condition() {
  $path = drupal_get_path('module', 'services_client') . '/include';

  $info = array();
  $info['ServicesClientPropertyCondition'] = array(
    'name' => t('Property'),
    'description' => 'Property condition',
    'handler' => array(
      'parent' => 'ServicesClientConditionPlugin',
      'class' => 'ServicesClientPropertyCondition',
      'file' => 'condition.inc',
      'path' => $path,
    ),
  );

  $info['ServicesClientFieldCondition'] = array(
    'name' => t('Field'),
    'description' => 'Field condition',
    'handler' => array(
      'parent' => 'ServicesClientConditionPlugin',
      'class' => 'ServicesClientFieldCondition',
      'file' => 'condition.inc',
      'path' => $path,
    ),
  );

  $info['ServicesClientUserRoleCondition'] = array(
    'name' => t('User Roles'),
    'description' => t('User roles condition'),
    'handler' => array(
      'parent' => 'ServicesClientConditionPlugin',
      'class' => 'ServicesClientUserRoleCondition',
      'file' => 'condition.inc',
      'path' => $path,
    ),
  );

  return $info;
}

/**
 * List availalable event handler plugins.
 *
 * @return array
 *   Plugins definition.
 */
function services_client_services_client_event_handler() {
  $path = drupal_get_path('module', 'services_client') . '/include';

  $info = array();
  $info['EntitySaveHandler'] = array(
    'name' => t('Entity save'),
    'description' => 'General entity save handler',
    'handler' => array(
      'parent' => 'EventHandler',
      'class' => 'EntitySaveHandler',
      'file' => 'event.inc',
      'path' => $path,
    ),
    'type' => 'save',
  );

  $info['NodeSaveHandler'] = array(
    'name' => t('Node save'),
    'description' => 'Node entity save handler',
    'handler' => array(
      'parent' => 'EntitySaveHandler',
      'class' => 'NodeSaveHandler',
      'file' => 'event.inc',
      'path' => $path,
    ),
    'type' => 'save',
  );

  $info['UserSaveHandler'] = array(
    'name' => t('User save'),
    'description' => 'User entity save handler',
    'handler' => array(
      'parent' => 'EntitySaveHandler',
      'class' => 'UserSaveHandler',
      'file' => 'event.inc',
      'path' => $path,
    ),
    'type' => 'save',
  );

  $info['EntityDeleteHandler'] = array(
    'name' => t('Entity delete'),
    'description' => 'General entity delete handler',
    'handler' => array(
      'parent' => 'EventHandler',
      'class' => 'EntityDeleteHandler',
      'file' => 'event.inc',
      'path' => $path,
    ),
    'type' => 'delete',
  );

  return $info;
}
