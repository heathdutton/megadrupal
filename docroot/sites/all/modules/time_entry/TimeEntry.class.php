<?php

include_once('TimeEntryType.class.php');

/**
 * The class used for timeentry entities.
 */
class TimeEntry extends Entity {

  /**
   * The profile id.
   *
   * @var integer
   */
  public $id;

  /**
   * The name of the time entry type.
   *
   * @var string
   */
  public $type;


  public $time;  
  public $duration;
  
  /**
   * The epoch when this entry ends, this field it's not saved to database but
   * it's processed every time we construct the entity to ease the live of
   * people using the module
   *
   * @var int
   */
  public $end; 

  public $created;
  public $log;

  public function __construct($values = array()) {
    if (!isset($this->type) && !isset($values['type'])) {
      $values['type'] = 'default';
    }
    
    if (!isset($values['duration']) && isset($values['end'])) {
      $values['duration'] = $values['end'] - $values['time'];
    }
    elseif (isset($values['duration']) && isset($values['time'])) {
      $values['end'] = $values['time'] + $values['duration'];
    }
    parent::__construct($values, 'time_entry');
  }

  /**
   * Returns the user owning this profile.
   */
  public function user() {
    return user_load($this->uid);
  }

  /**
   * Sets a new user owning this profile.
   *
   * @param $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  /**
   * Gets the associated profile type object.
   *
   * @return ProfileType
   */
  public function type() {
   // return profile2_get_types($this->type);
  }

  /**
   * Returns the full url() for the profile.
   */
  public function url() {
    $uri = $this->uri();
    return url($uri['path'], $uri);
  }

  /**
   * Returns the drupal path to this profile.
   */
  public function path() {
    $uri = $this->uri();
    return $uri['path'];
  }

  public function defaultUri() {
    return array(
      'path' => 'time_entry/' . $this->id,
//      'options' => array('fragment' => 'profile-' . $this->type), ??
    );
  }

  // ??^2
  // public function buildContent($view_mode = 'full', $langcode = NULL) {
  //     $content = array();
  //     // Assume newly create objects are still empty.
  //     if (!empty($this->is_new)) {
  //       $content['empty']['#markup'] = '<em class="profile2-no-data">' . t('There is no profile data yet.') . '</em>';
  //     }
  //     return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  //   }

  public function save() {
    if (!isset($this->created)) {
      $this->created = time();
    }
    if (!isset($this->log)) {
      $this->log = '';
    }
    parent::save();
  }

  public function delete() {
    parent::delete();
  }
  
  public function getStartDateTime() {
    return new DateTime('@'. $this->time);
  }
  
  public function getEndDateTime() {
    return new DateTime('@'. ($this->time+$this->duration));
  }
  
  public function toDateTime() {
    return array(
      'value' => $this->getStartDateTime(),
      'value2' => $this->getEndDateTime(),
    );
  }
  
  /*
   * Merge a given entry to another one.
   *
   * This function checks if both entities have the same type, and clones the
   * one the functions has been called upon (i.e. $this), so all fields from
   * the first one will be copied over.  $id is not updated.
   */
  public function merge(TimeEntry $entry) {
    if ($this->type != $entry->type) {
      return FALSE;
    }

    $result = clone $this;

    if ($this->time < $entry->time) {
      $first = TRUE;
      $result->time = $this->time;
    }
    else {
      $first = FALSE;
      $result->time = $entry->time;
    }

    if (($first && ($this->end < $entry->time -1)) ||
       (!$first && ($entry->end < $this->time -1))) {
      // This is no intersection and the entries are not continuos, abort!
      return FALSE;
    }

    $result->end = $this->end < $entry->end? $entry->end : $this->end;
    $result->duration = $result->end - $result->time;

    $result->merged = TRUE;
    return $result;
  }

}

