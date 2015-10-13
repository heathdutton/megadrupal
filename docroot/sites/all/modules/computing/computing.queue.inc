<?php

/**
 * This file provides Queue support, an extra layer to handle computing records.
 * The only benefit here is to provide an interface for Queue, perhaps people don't need it.
 */

class ComputingQueue implements DrupalReliableQueueInterface {

  protected $app_name;

  // singleton design pattern.
  private function __construct($app_name) {
    // we don't validate $app_name against "computing_application" entity.
    $this->app_name = $app_name;
  }

  /**
   * Factory method.
   * @param $app_name
   * @return object
   */
  public static function get($app_name) {
    static $queues;
    if (!isset($queues[$app_name])) {
      $object = new ComputingQueue($app_name);
      $queues[$app_name] = $object;
    }
    return $queues[$app_name];
  }

  ///////////////////////////// below are methods defined in DrupalReliableQueueInterface

  public function createItem($data) {
    if (is_object($data)) {
      $data = (array) $data;
    }
    $options = array_diff_key($data, array_flip(array('application', 'command', 'label', 'input')));
    return (bool) computing_create($data['application'], $data['command'], $data['label'], $data['input'], $options);
  }


  public function numberOfItems() {
    return db_query("SELECT COUNT(id) FROM {computing_record} WHERE application = :name AND status = 'RDY'", array(':name' => $this->app_name))->fetchField();
  }


  public function claimItem($lease_time = 3600) {
    //$lease_time: not used.
    return computing_claim($this->app_name);
  }

  public function deleteItem($item) {
    // we'll not physically delete the item, but just mark the item as "ABD".
    if (!is_object($item)) {
      $item = (object) $item;
    }

    // this is to follow the definition in the interface.
    if (computing_update_field($item->id, 'status', 'ABD')) {
      return computing_load($item->id);
    }
    else {
      return FALSE;
    }
  }


  public function releaseItem($item) {
    if (!is_object($item)) {
      $item = (object) $item;
    }
    return computing_release($item->id);
  }


  public function createQueue() {
    // nothing to be done here as the 'computing' table/entity_type is created in installation.
  }

  public function deleteQueue() {
    // we won't delete any data here. keep everything for archival purpose.
  }
}