<?php

/**
 * @file
 * Defines and processes CollectiveAccess list_items
 */

class CollectiveAccessListItem {
  protected $data;
  protected $item_id;
  protected $collectiveaccess_instance;
  protected $options;

  /**
   * Create a new CollectiveAccessListItem
   * @param CollectiveAccessInstance $ca - object connecting to CollectiveAccess installation
   * @param int $item_id - id of ca_list_items entry to retrieve
   * @param array $options - array with options. Possible values:
   *   * 'skeleton' => TRUE/FALSE (default FALSE) - if enabled, object can only be used as skeleton. No data will be loaded automatically.
   */
  public function __construct(CollectiveAccessInstance $ca, $item_id, $options = array()) {
    $this->collectiveaccess_instance = $ca;
    $this->item_id = $item_id;
    $this->data['item_id'] = $item_id;
    $this->options = $options;

    if (!isset($options['skeleton']) || $options['skeleton'] != TRUE) {
      $this->getItem();
    }
  }

  /**
   * Magic method to retrieve CollectiveAccess ca_list_items entry field
   */
  public function __get($name) {
    if (array_key_exists($name, $this->data)) {
      return $this->data[$name];
    }
  }

  /**
   * Magic method to set CollectiveAccess ca_list_items entry field
   */
  public function __set($name, $value) {
    $this->data[$name] = $value;
  }

  /**
   * Get all ca_list_items entry fields from the CollectiveAccess webservice
   */
  protected function getItem() {
    $params = array('type' => 'ca_list_items', 'item_id' => $this->item_id);
    $data = $this->collectiveaccess_instance->execute('ItemInfo', 'getItem', $params);
    if ($data) {
      foreach ($data as $k => $v) {
        $this->$k = $v;
      }
    }
  }

  /**
   * Get preferred labels for this ca_list_items entry, grouped by locale
   */
  public function getPreferredLabels() {
    $ret = array();
    $params = array(
      'type' => 'ca_list_items',
      'item_id' => $this->item_id,
      'mode' => 'all',
    );
    $labels = $this->collectiveaccess_instance->execute('ItemInfo', 'getLabels', $params);
    if (is_array($labels)) {
      $locale_labels = array_shift($labels); // only get preferred labels
      if ($locale_labels) {
        foreach ($locale_labels as $label) {
          $label_val = array_shift($label);
          $locale = ($label_val['locale_id'] >= 1) ? $label_val['locale_language'] . '_' . $label_val['locale_country'] : 'NONE';
          $ret[$locale] = $label_val['name_plural'];
        }
      }
    }
    return $ret;
  }
}

/**
 * CollectiveAccessListItemBuilder helps building CollectiveAccessListItem objects
 */
class CollectiveAccessListItemBuilder {
  protected $collectiveaccess_instance;

  /**
   * Constructor for CollectiveAccessListItemBuilder
   */
  public function __construct(CollectiveAccessInstance $ca) {
    $this->collectiveaccess_instance = $ca;
  }

  /**
   * This helper method tries to get a CollectiveAccessListItem from static cache if available
   * Loads the object normally if it's not available.
   */
  public function getCachedListItemObject($item_id, $options = array()) {
    static $labels;
    if (!isset($labels[$item_id])) {
      $ca_list_item = new CollectiveAccessListItem($this->collectiveaccess_instance, $item_id, $options);
      $labels[$item_id] = $ca_list_item->getPreferredLabels();
    }
    return $labels[$item_id];
  }
}