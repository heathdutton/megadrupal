<?php

/**
 * @file
 * Services client allows you to push different objects from local drupal installation
 * to remote servers via REST api.
 */

/**
 * Alter list of available plugins.
 *
 * @param array $plugins
 *   List of all currently available plugins
 * @param string $type
 *   Type of required plugins
 */
function hook_services_client_plugins_alter($plugins, $type) {
  if (isset($plugins['MyType'])) {
    // Alter plugin definition.
  }
}

/**
 * ctools hook for Mapping plugin discover.
 *
 * @see services_client_services_client_mapping
 *
 * @return array
 *   Ctools plugin definition.
 */
function hook_services_client_mapping() {
  $info = array();

  $info['MyPlugin'] = array(
    'name' => t('My plugin'),
    'description' => 'My reader/writer plugin',
    'handler' => array(
      'parent' => 'ServicesClientMapperPlugin',
      'class' => 'MyPlugin',
      'file' => 'MyPlugin.inc',
      'path' => drupal_get_path('module', 'mymodule') . '/plugins',
    ),
    // This is important to define weather this is reader or formatter plugin
    'type' => 'reader',
  );

  return $info;
}

/**
 * ctools hook for Condition plugin discover.
 *
 * @see services_client_services_client_condition
 *
 * @return array
 *   Ctools plugin definition.
 */
function hook_services_client_condition() {
  $info = array();

  $info['ConditionPlugin'] = array(
    'name' => t('Condition plugin'),
    'description' => 'My condition plugin',
    'handler' => array(
      'parent' => 'ServicesClientConditionPlugin',
      'class' => 'ConditionPlugin',
      'file' => 'ConditionPlugin.inc',
      'path' => drupal_get_path('module', 'mymodule') . '/plugins',
    ),
  );

  return $info;
}

/**
 * ctools hook for Event Hanlder plugin discover.
 *
 * @see services_client_services_client_event_handler
 *
 * @return array
 *   Ctools plugin definition.
 */
function hook_services_client_event_handler() {
  $info = array();

  $info['MyEntityTypeEventHandler'] = array(
    'name' => t('My entity save handler'),
    'description' => 'Specific for custom entity',
    'handler' => array(
      'parent' => 'EntitySaveHandler',
      'class' => 'MyEntityTypeEventHandler',
      'file' => 'MyEntityTypeEventHandler.inc',
      'path' => drupal_get_path('module', 'mymodule') . '/plugins',
    ),
    // It's important to define whether this is save/delete handler.
    'type' => 'save',
  );

  return $info;
}

/**
 * Allow to alter object that is sent to remote site.
 *
 * @param EventHandler $handler
 *   Handler class that provides sending object.
 *
 * @param stdClass $object
 *   Object that is going to be sent.
 */
function hook_services_client_mapped_object_alter($handler, $object) {
  if ($handler->getEvent()->name == 'my_event') {
    // Load entity that is being processed.
    $entity = $handler->getEntity();
    $object->my_property = $entity->custom_array['value'];
  }
}

/**
 * Allows to react on push event before its executed.
 *
 * @param EventHandler $handler
 *   Handler class that provides sending object.
 *
 * @param stdClass $object
 *   Object that is going to be sent.
 */
function hook_services_client_before_request($handler, $object) {

}

/**
 * Allows to react on push event after its executed.
 *
 * @param EventHandler $handler
 *   Handler class that provides sending object.
 *
 * @param stdClass $object
 *   Object that is going to be sent.
 *
 * @param ServicesClientEventResult $result
 *   Object describing operation result.
 */
function hook_services_client_after_request($handler, $object, $result) {

}

/**
 * Allows to process sync results.
 *
 * @param array $results
 *   An array of ServicesClientEventResult objects. Each of these objects
 *   represents single operation executed by services_client module.
 */
function hook_services_client_process_events($results) {

}

/**
 * This hook allows to prevent syncing entity by automatic services_client
 * event handling.
 *
 * @param EventHandler $handler
 *   Handler class that provides sending object.
 *
 * @param stdClass $entity
 *   Local entity object that should be processed.
 *
 * @param string $entity_type
 *   Entity type name.
 *
 * @return bool
 *   TRUE if entity shouldn't be automatically synced to remote site.
 */
function hook_services_client_skip_autosync($handler, $entity, $entity_type) {
  // This would prevent syncing uid 1 account by default.
  if ($entity_type == 'user' && $entity->uid == 1) {
    return TRUE;
  }
}
