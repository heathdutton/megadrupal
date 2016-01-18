<?php

/**
 * @file
 * Defines and processes a base CollectiveAccess item
 */

abstract class CollectiveAccessBaseItem {
  protected $data;
  protected $collectiveaccess_instance;
  protected $options;
  protected $id;

  /**
   * Create a new CollectiveAccessBaseItem
   * @param CollectiveAccessInstance $ca - object connecting to CollectiveAccess installation
   * @param int $id - id of ca_entities to retrieve
   * @param array $options - array with options
   */
  public function __construct(CollectiveAccessInstance $ca, $id, $options = array()) {
    $this->collectiveaccess_instance = $ca;
    $this->id = $id;
    $this->options = $options;
  }

  /**
   * Magic method to retrieve CollectiveAccess entity attribute
   */
  public function __get($name) {
    if (array_key_exists($name, $this->data)) {
      return $this->data[$name];
    }
  }

  /**
   * Magic method to set CollectiveAccess entity attribute
   */
  public function __set($name, $value) {
    $this->data[$name] = $value;
  }

  /**
   * Get all CollectiveAccess data as a raw array
   * @return array list of attribute data
   */
  public function getData() {
    return $this->data;
  }

  /**
   * Get a list of available attribute keys
   */
  public function getAttributes() {
    if (is_array($this->data)) {
      return array_keys($this->data);
    }
    return FALSE;
  }

  /**
   * Get the CollectiveAccessInstance object
   */
  public function getCAInstance() {
    return $this->collectiveaccess_instance;
  }

  /**
   * Manipulate Attribute
   *
   * This method reviews the available attributes and imports them whenever
   * necessary into this object, including their labels.
   *
   * @param   val     Value
   * @param   key     Key
   * @param   type    Type
   * @return  array   Array of Attributes
   */
  protected function manipulate_attribute($key, $val, $type, $ca_type) {
    // allow other modules to manipulate the value
    drupal_alter('collectiveaccess_attribute', $val, $key, $type);
    // fetch list item labels if requested
    if ($val && $type == "List" && isset($this->options['list_labels']) && $this->options['list_labels'] == TRUE) {
      foreach ($val as $locale => $values) {
        foreach ($values as $k => $v) {
          $data = $v[$key];
          $data_arr = explode(':', $data);
          if (is_numeric($data_arr[0])) {
            $options = array('skeleton' => TRUE);
            $ca_list_item = new CollectiveAccessListItem($this->getCAInstance(), $data_arr[0], $options);
            $labels = $ca_list_item->getPreferredLabels();
            $val[$locale][$k]['labels'] = $labels;
          }
        }
      }
    }
    // fetch File labels
    if ($val && $type == "File") {
      foreach ($val as $locale => $values) {
        $labels = $this->fetchFileLabels($key, $ca_type);
        $counter = 0;
        foreach ($values as $value_k => $value_v) {
          $val[$locale][$value_k]['label'] = $labels[$counter];
          $counter++;
        }
      }
    }
    return $val;
  }

  /**
   * Fetch labels for ca_attributes of type File
   */
  protected function fetchFileLabels($attribute_name, $ca_type) {
    $labels = array();
    $params = array('type' => $ca_type, 'item_id' => $this->id, 'attribute_code_or_id' => $attribute_name);
    $data = $this->collectiveaccess_instance->execute('ItemInfo', 'getAttributesByElement', $params);
    foreach ($data as $k => $v) {
      $labels[] = $v[0]['display_value'];
    }
    return $labels;
  }

  /**
   * Fetch basic object data and user-defined fields and assign them as attributes to this object
   *
   * @param     type      The type of content to fill in, e.g. ca_objects, ca_places, ...
   *                      This corresponds to the types in CollectiveAccess
   */
  protected function fetchData($type) {
    $params = array('type' => $type, 'item_id' => $this->id);
    $data = $this->collectiveaccess_instance->execute('ItemInfo', 'getItem', $params);
    $attributes = $this->collectiveaccess_instance->getAvailableAttributes($type);

    // Loop over the data to assign attributes to this object
    // @TODO: configure access IDs to check against in entity settings
    if ($data && $data["access"] != 0) {
      foreach ($data as $attrib_key => $attrib_value) {
        // get user-defined attributes
        if (substr($attrib_key, 0, 13) == 'ca_attribute_') {
          $attribute_name = str_replace('ca_attribute_', '', $attrib_key);
          if (!array_key_exists($attribute_name, $attributes)) {
            return FALSE;
          }
          $atype = $attributes[$attribute_name];
          $this->data[$attribute_name] = $this->manipulate_attribute($attribute_name, $attrib_value, $atype, $type);
        }
        // get basic attributes
        else {
          $this->data[$attrib_key] = $attrib_value;
        }
      }

      return TRUE;
    }
    return FALSE;
  }

  /**
   *
   * GetLabels
   */
  public function getLabels($mode = 'preferred', $type = 'ca_objects') {
    $params = array('type' => $type, 'item_id' => $this->getId(), 'mode' => $mode);
    $labels = $this->collectiveaccess_instance->execute('ItemInfo', 'getLabels', $params);
    return $labels;
  }

  /**
   * Getter for the ID
   *
   */
  public function getID() {
    return $this->id;
  }

  /**
   *
   * @TODO: Make Abstract
   *
   */
  abstract function getType();

}



/**
 * CollectiveAccessBaseItemBuilder can be used to retrieve ID's of
 * recently changed objects or ID's of stuff belonging to a specific
 * collection
 *
 *
 */
class CollectiveAccessBaseItemBuilder {

  /**
   * Get a list of object IDs that changed since given timestamp
   *
   * @param   connector   CollectiveAccessInstance to connect to
   * @param   type        The type of item you wish to retrieve IDs for
   * @param   timestamp   Retrieve items that have been changed since this time
   * @return  array       object IDs
   */
  public static function getLastChangedItemIDs($connector, $type, $timestamp = 0) {
    $result = array();
    $params = array('type' => $type, 'timestamp' => $timestamp);
    $changed = $connector->execute('ItemInfo', 'getLastChangedItems', $params);
    if ($changed) {
      foreach ($changed as $object_id => $timestamp) {
        $result[$object_id] = $timestamp;
      }
    }
    return $result;
  }
}


/**
 * CollectiveAccessLocaleBuilder helps retrieving information about the
 * locales currently in use
 *
 */
class CollectiveAccessLocaleBuilder {

  protected $collectiveaccess_instance;

  /**
   * Constructor for CollectiveAccessLocaleBuilder
   */
  public function __construct(CollectiveAccessInstance $ca) {
    $this->collectiveaccess_instance = $ca;
  }


  /**
   * Gets the locales available in CollectiveAccess
   *
   * @param $available_for_cataloguing_only Show only cataloguing locales
   * @return an array of locale codes (e.g. en_US)
   */
  public function getLocaleList($available_for_cataloguing_only = false) {
    //$params = array('options' => array('dont_use_for_cataloguing' => '0'));
    $params = array();
    $result = $this->collectiveaccess_instance->execute('ItemInfo', 'getLocaleList', $params);

    $locales = array();
    if ($result) {
      foreach ($result as $id => $object) {
        // Filter non-cataloguing locales
        if (!$available_for_cataloguing_only || $object['dont_use_for_cataloguing'] == 0) {
          $locales[$object['code']] = $object;
        }
      }
    }
    return $locales;
  }
}