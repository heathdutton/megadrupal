<?php

/**
 * @file
 * Contains the AchieverActivity class.
 */

/**
 * Class AchieverActivity
 *
 * The class used by the Achiever module to track user activities.
 *
 * Developers are discouraged to manually instantiate this class, but should
 * but should rely on achiever_register_activity() API function for ease of use
 * and to prevent any breakage due to future inner-api changes.
 */
class AchieverActivity {

  private $uid;
  private $namespace;
  private $type;
  private $variant;
  private $value;
  private $date;
  private $data;
  private $account;

  /**
   * Constructor.
   *
   * @param int $uid
   *  The id of the user owner of the activity.
   * @param string $namespace
   *  The namespace of the activity.
   * @param string $type
   *  The type of the activity.
   * @param string $variant
   *  The variant of the activity.
   * @param float $value
   *  The value of the activity.
   * @param int $date
   *  The date of the activity in unix timestamp format.
   * @param mixed $data
   *  Any data that is to be used as context by systems observing activities.
   * @param object $account
   *  The account belonging to the user responsible for the activity.
   */
  public function __construct($uid, $namespace, $type, $variant, $value, $date, $data, $account) {
    $this->uid = $uid;
    $this->namespace = $namespace;
    $this->type = $type;
    $this->variant = $variant;
    $this->value = $value;
    $this->date = $date;
    $this->data = $data;
    $this->account = $account;
  }

  /**
   * Gets the user id.
   *
   * @return int
   *  The user ID
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * Gets the activity namespace.
   *
   * @return string
   *  The activity namespace.
   */
  public function getNamespace() {
    return $this->namespace;
  }

  /**
   * Gets the activity type.
   *
   * @return string
   *  The activity type.
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Gets the activity variant.
   *
   * @return string
   *  The activity variant.
   */
  public function getVariant() {
    return $this->variant;
  }

  /**
   * Gets the activity value.
   *
   * @return int
   *  The activity value.
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Gets the activity date.
   *
   * @return int
   *  The activity date in timestamp format.
   */
  public function getDate() {
    return $this->date;
  }

  /**
   * Gets the activity contextual data.
   *
   * @return mixed
   *  The activity data. Can be anything.
   */
  public function getData() {
    return $this->data;
  }

  /**
   * Gets the activity user.
   *
   * @return object
   *  The user account object.
   */
  public function getAccount() {
    return $this->account;
  }

}
