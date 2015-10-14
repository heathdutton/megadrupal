<?php

namespace Drupal\campaignion_newsletters;

class QueueItem extends \Drupal\little_helpers\DB\Model {
  const WELCOME = 4;
  const OPTIN = 2;
  const SUBSCRIBE = 1;
  const UNSUBSCRIBE = 0;

  public $list_id;
  public $email;
  public $created;
  public $locked = 0;
  public $action;
  public $data;
  public $fingerprint;

  protected static $table = 'campaignion_newsletters_queue';
  protected static $key = array('list_id', 'email');
  protected static $values = array('created', 'locked', 'action', 'data');
  protected static $serialize = array('data' => TRUE);
  protected static $serial = FALSE;

  public static function load($list_id, $email) {
    $table = static::$table;
    $keys = array(':list_id' => $list_id, ':email' => $email);
    $result = db_query("SELECT * FROM {{$table}} WHERE list_id=:list_id AND email=:email", $keys);
    if ($row = $result->fetch()) {
      return new static($row, FALSE);
    }
  }

  public static function byData($data) {
    if ($item = static::load($data['list_id'], $data['email'])) {
      $item->__construct($data, FALSE);
    }
    else {
      $item = new static($data);
    }
    return $item;
  }

  public static function claimOldest($limit, $time = 600) {
    $table = static::$table;
    $result = db_select(static::$table, 'i')
      ->fields('i')
      ->orderBy('created')
      ->condition('locked', time(), '<')
      ->range(0, $limit)
      ->execute();
    $items = array();
    foreach ($result as $row) {
      $item = new static($row, FALSE);
      $item->claim($time);
      $items[] = $item;
    }
    return $items;
  }

  public function __construct($data = array(), $new = TRUE) {
    parent::__construct($data, $new);
    if (!isset($this->created)) {
      $this->created = time();
    }
  }

  /**
   * Lock this item for $time seconds.
   *
   * @param int $time
   *   Seconds to lock this item for.
   */
  public function claim($time = 600) {
    $this->locked = time() + $time;
    $this->save();
  }

  /**
   * Release the lock on this item.
   */
  public function release() {
    $this->locked = 0;
    $this->save();
  }
}
