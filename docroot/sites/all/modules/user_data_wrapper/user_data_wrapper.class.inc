<?php
/**
 * @file
 * Classes to control user->data.
 */

/**
 * Class user_data.
 */
class UserDataWrapper {
  protected $account = NULL;
  private $baseKey = 'user_data';
  private $key = NULL;

  /**
   * UserDataWrapper construct.
   *
   * @param object $account
   *    User object to use.
   * @param string $key
   *    The property key to store the data against.
   */
  public function __construct($account, $key) {
    $this->key = $key;

    // Test is a "demi" user object is passed, if so load full user object.
    if (!isset($account->data)) {
      $account = user_load($account->uid);
    }
    $this->account = $account;
    // Make sure $account->data is an array.
    if (!is_array($this->account->data)) {
      // Move any existing data to unknown so it is not totally lost.
      $this->account->data = $this->account->data?array('unknown' => $this->account->data):array();
    }

    // Initiate the base key for this user.
    if (!array_key_exists($this->baseKey, $this->account->data)) {
      $this->account->data[$this->baseKey] = array();
    }

    if (!array_key_exists($this->key, $this->account->data[$this->baseKey])) {
      $this->account->data[$this->baseKey][$this->key] = NULL;
    }
  }

  /**
   * Sets value with baseKey and property key.
   *
   * @param mixed $value
   *    Value to store for property.
   */
  public
  function setData($value) {
    $this->account->data[$this->baseKey][$this->key] = $value;
  }

  /**
   * Gets data using base key and property key.
   *
   * @return mixed
   *    Data stored for property.
   */
  public
  function getData() {
    return $this->account->data[$this->baseKey][$this->key];
  }

}
