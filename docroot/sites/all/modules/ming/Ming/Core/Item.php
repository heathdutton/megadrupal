<?php
/**
 * @file
 * A base class for Items to be managed in and out of the Ming system.
 */
namespace Ming\Core;

/**
 * Base class for an Item
 */
class Item {
  /**
   * Mongo ID
   *
   * @var string
   */
  protected $mid;

  /**
   * Properties to be written to the database
   *
   * @var array
   */
  protected $properties;

  /**
   * Collection to use
   *
   * @var string
   */
  protected $collection;

  /**
   * The active database connection
   *
   * @var \Ming\Core\Database
   */
  protected $db;

  /**
   * Constructor
   */
  public function __construct(){
    // Nothing to do currently
  }

  /**
   * Set mid
   *
   * This will check whether the provided ID is a MongoId object before setting
   * the value.
   *
   * @param $id
   */
  public function setIdentifier($id) {
    if(is_object($id) && get_class($id) == 'MongoId') {
      $this->mid = $id['$id'];
    }
    else {
      $this->mid = $id;
    }
  }

  /**
   * Set a property
   *
   * @param $key
   * @param $value
   */
  public function set($key, $value) {
    $this->properties[$key] = $value;
  }

  /**
   * Unset a property
   *
   * @param $key
   */
  public function remove($key) {
    if (isset($this->properties[$key])) {
      unset($this->properties[$key]);
    }
  }

  /**
   * Save the current item to the database.
   *
   * This will either create or update an existing item, using the Mongo upsert
   * functionality. It also sets time created (if new) and time updated.
   *
   * @param null $connection
   * The connection alias to use
   *
   * @return array
   */
  public function save($connection = NULL) {
    // Set default
    if (!isset($this->properties['time_created'])) {
      $this->set('time_created', time());
    }
    $this->set('time_updated', time());

    $this->db();

    if (isset($this->mid)) {
      $filter = $this->db->filterID($this->mid);
    }
    else {
      $filter = array();
    }

    return $this->db->upsert($filter, $this->items());
  }

  /**
   * Access a Ming DB connection
   *
   * @param string $reset
   *  If TRUE, reload the connection from source, otherwise, use an existing
   *  connection if available. Note, this only reattaches the Ming database
   *  currently in use to THIS object, it has no effect on the connection itself
   *  or its persistence.
   *
   * @return \Ming\Core\Database
   *
   * @visibility public
   */
  public function db($reset = FALSE) {
    if (!isset($this->db) || empty($this->db) || $reset == TRUE) {
      $this->db = ming_db();
    }
    $this->db->collection($this->collection());

    return $this->db;
  }

  /**
   * Set the collection to use for this item.
   *
   * Most objects which inherit Item will normally define this for themselves.
   *
   * @param string $c_name [optional]
   *  If provided, the collection in use will be set to the string provided.
   *
   * @return string | bool
   *  If the collection name is set, returns the collection name, otherwise FALSE.
   */
  public function collectionName($c_name = NULL) {
    if (!empty($c_name)) {
      $this->collection = $c_name;
    }
    return $this->collection();
  }

  /**
   * Returns the current collection name
   *
   * @return string | bool
   *  Either the name of the collection, or FALSE.
   */
  public function collection() {
    if (!empty($this->collection)) {
      return $this->collection;
    }

    return FALSE;
  }

  /**
   * Return an objects properties as an array for inserting into Mongo
   *
   * @return array
   */
  public function items() {
    return $this->properties;
  }

  /**
   * Magic method to get object properties
   *
   * @param $name
   * @return mixed
   */
  function __get($name) {
    if (isset($this->properties[$name])) {
      return $this->properties[$name];
    }
    return FALSE;
  }

  /**
   * Magic method to set object properties
   *
   * @param $name
   * @param $value
   */
  function __set($name, $value) {
    $this->properties[$name] = $value;
  }

  /**
   * Magic method to check isset on object properties
   *
   * @param $name
   * @return bool
   */
  function __isset($name) {
    if (isset($this->properties[$name])) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Magic method to unset object properties
   *
   * @param $name
   */
  function __unset($name) {
    if (isset($this->properties[$name])) {
      unset($this->properties[$name]);
    }
  }

}
