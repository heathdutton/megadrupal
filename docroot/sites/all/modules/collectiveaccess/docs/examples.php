<?php
// $Id$

/**
 * @file
 * example code on how to interact with CollectiveAccess through code
 */

/**
 * Example 1: Load a CollectiveAccess instance
 * Remark: The name and settings for a given instance are typically stored
 * via the collectiveaccess_ui interface or collectiveaccess_instance_settings_save()
 */

function example_collectiveaccess_load_instance() {
  $ca_instance = collectiveaccess_instance_load('instance_name');
}

/**
 * Example 2: Retrieve basic CollectiveAccess object data through factory classes
 */

function example_collectiveaccess_get_object_data() {
  // load CA instance (see Example 1)
  $ca_instance = collectiveaccess_instance_load('instance_name');
  $factory = CollectiveAccessObjectFactory($ca_instance);
  $object = $factory->createBasicObject(12); // get ca_item_id 12 basic info
}

/**
 * Example 3: Retrieve CollectiveAccess objects in a particular set
 */

function example_collectiveaccess_get_set_objects() {
  $objects = array();
  // load CA instance (see Example 1)
  $ca_instance = collectiveaccess_instance_load('instance_name');
  $set_items = CollectiveAccessItemInfo::getSetItems($ca_instance, 1); // setid = 1

  $factory = CollectiveAccessObjectFactory($ca_instance);
  if ($set_items) {
    foreach ($set_items as $basic_info) {
      $ca_item_id = $basic_info['idno'];
      $objects[] = $factory->create($ca_item_id);
    }
  }
  // now do something with the $objects
}

/**
 * Example 4: connect to CollectiveAccess via SOAP
 * Remark: this example shows how to execute low level API calls on the CollectiveAccess object
 * For more high level API functionality, use the collectiveaccess_instance_load()
 */
function example_collectiveaccess_soap_connection() {
  $settings = array();
  $settings['service_path'] = 'http://mycollectiveaccess.install/service.php';

  $soap = new CollectiveAccessSOAPConnector($settings);

  $ca = new CollectiveAccess($soap);
  $params = array(
    'type' => 'ca_object',
    'item_id' => 'ITEM_001',
  );
  $result = $ca->execute('ItemInfo', 'getItem', $params);
}