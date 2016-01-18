<?php

/**
 * @file
 * Defines and processes CollectiveAccess entities
 */

class CollectiveAccessCollection extends CollectiveAccessBaseItem {

  /**
   * Create a new CollectiveAccessCollection
   *
   * This is a representation of Occurrences in ColllectiveAccess,
   * most likely representing content like 'Events', 'Periods', anything that
   * has a clear date or date period.
   * 
   * @param     $ca                 CollectiveAccessInstance: connecting to CollectiveAccess installation
   * @param     int $id             entity_id of ca_entities to retrieve
   * @param     array $options      array with options. Possible values:
   *   * 'skeleton' => TRUE/FALSE (default FALSE) - if enabled, object can only be used as skeleton. No data will be loaded automatically.
   *   * 'list_labels' => TRUE/FALSE - if enabled, fetch preferred labels for list items
   */
  public function __construct(CollectiveAccessInstance $ca, $id, $options = array()) {
    parent::__construct($ca, $id, $options);
    $this->data['collection_id'] = $id;
    // set defaults for $options
    if (!isset($options['list_labels'])) {
      $options['list_labels'] = TRUE;
    }
    $this->options = $options;

    if (!isset($options['skeleton']) || $options['skeleton'] != TRUE) {
      $this->fetchData();
    }
  }
  
  /**
   * Overrides fetchData from parent
   *
   */
  public function fetchData() {
    return parent::fetchData('ca_collections');
  }
  
  /**
   * Overrides getLabels from parent
   *
   */
  public function getLabels($mode = 'preferred') {
    return parent::getLabels($mode, 'ca_collections');
  }
  
  public function getType() {
    return 'ca_collections';
  }
  
  public function __get($name) {
    if (array_key_exists($name, $this->data)) {
      return $this->data[$name];
    }
  }

}