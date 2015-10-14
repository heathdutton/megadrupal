<?php

namespace Drupal\campaignion_activity;

class ActivityBase implements ActivityInterface {
  protected $activity_id = NULL;
  protected $contact_id;
  protected $type;
  protected $created;

  public function __construct($data = array()) {
    foreach ($data as $k => &$v) {
      $this->{$k} = &$v;
    }
    if (!isset($this->created)) {
      $this->created = time();
    }
  }
  
  public static function load($activity_id) {
    return db_select('campaignion_activity', 'a')
      ->fields('a')
      ->condition('activity_id', $activity_id)
      ->execute()
      ->fetchObject(get_called_class());
  }
  
  public function save() {
    if (isset($this->activity_id)) {
      $this->update();
    } else {
      $this->insert();
    }
    // Let other modules react on saving an new activity.
    module_invoke_all('campaignion_activity_save', $this);
  }

  public function delete() {
    if (isset($this->activity_id)) {
      db_delete('campaignion_activity')->condition('activity_id', $this->activity_id)->execute();
    }
  }

  protected function values($keys) {
    $data = array();
    foreach ($keys as $k) {
      $data[$k] = isset($this->{$k}) ? $this->{$k} : NULL;
    }
    return $data;
  }

  protected function update() {
    db_update('campaignion_activity')
      ->condition('activity_id', $this->activity_id)
      ->fields($this->values(array('contact_id', 'type', 'created')))
      ->execute();
  }
  protected function insert() {
    $this->activity_id = db_insert('campaignion_activity')
      ->fields($this->values(array('contact_id', 'type', 'created')))
      ->execute();
  }

  public function contact() {
    return Contact::load($this->contact_id);
  }
}
