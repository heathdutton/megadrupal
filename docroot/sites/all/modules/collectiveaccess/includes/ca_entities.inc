<?php

/**
 * @file
 * Defines and processes CollectiveAccess entities
 */

class CollectiveAccessEntity extends CollectiveAccessBaseItem {

  /**
   * Create a new CollectiveAccessEntity
   * @param CollectiveAccessInstance $ca -object connecting to CollectiveAccess installation
   * @param int $id - entity_id of ca_entities to retrieve
   * @param array $options - array with options. Possible values:
   *   * 'skeleton' => TRUE/FALSE (default FALSE) - if enabled, object can only be used as skeleton. No data will be loaded automatically.
   *   * 'list_labels' => TRUE/FALSE - if enabled, fetch preferred labels for list items
   */
  public function __construct(CollectiveAccessInstance $ca, $id, $options = array()) {
    parent::__construct($ca, $id, $options);
    $this->data['entity_id'] = $id;
    // set defaults for $options
    if (!isset($options['list_labels'])) {
      $options['list_labels'] = TRUE;
    }
    $this->options = $options;

    if (!isset($options['skeleton']) || $options['skeleton'] != TRUE) {
      $this->fetchData();
    }
  }

  public function fetchData() {
    $params = array('type' => 'ca_entities', 'item_id' => $this->entity_id);
    $data = $this->collectiveaccess_instance->execute('ItemInfo', 'getItem', $params);
    $attributes = $this->collectiveaccess_instance->getAvailableAttributes('ca_entities');

    // Loop over the data to assign attributes to this object
    // @TODO: configure access IDs to check against in entity settings
    //if ($data && $data["access"] != 0) {
    if ($data) {
      foreach ($data as $attrib_key => $attrib_value) {
        // get user-defined attributes
        if (substr($attrib_key, 0, 13) == 'ca_attribute_') {
          $attribute_name = str_replace('ca_attribute_', '', $attrib_key);
          $type = $attributes[$attribute_name];
          $this->$attribute_name = $this->manipulate_attribute($attribute_name, $attrib_value, $type, 'ca_entities');
        }
        // get basic attributes
        else {
          $this->$attrib_key = $attrib_value;
        }
      }
      return TRUE;
    }
    return FALSE;
  }

  public function getType() {
    return 'ca_entities';
  }

}