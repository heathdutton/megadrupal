<?php

/**
 * Class Membersify_Object
 */
class Membersify_Object
{
  protected static $object = 'object';

  /**
   * Constructor method.
   *
   * @param null $id
   *   The id of the object.
   */
  public function __construct($id = null)
  {
    $this->id = $id;
  }

  /**
   * Stringifies the object in JSON.
   *
   * @return string
   */
  public function __toJSON()
  {
    if (defined('JSON_PRETTY_PRINT'))
      return json_encode($this->__toArray(true), JSON_PRETTY_PRINT);
    else
      return json_encode($this->__toArray(true));
  }

  /**
   * Alias for __toJSON().
   *
   * @return string
   */
  public function __toString()
  {
    return $this->__toJSON();
  }

  /**
   * Returns an associative array for the keys/values in the object.
   *
   * @return array
   */
  public function __toArray() {
    $data = array();

    foreach ($this as $key => $value) {
      $data[$key] = $value;
    }

    return $data;
  }

  /**
   * Returns the current class name of the object.
   *
   * @return string
   */
  public static function get_class_name() {
    return get_called_class();
  }
}
