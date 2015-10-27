<?php

abstract class ZuoraObject implements ZuoraObjectInterface {

  /**
   * Initiate an API object with an array of data.
   *
   * @param array $data
   */
  public function __construct(array $data = array()) {
    foreach ($data as $key => $value) {
      $this->{$key} = $value;
    }
  }

  /**
   * Provide magic method for accessing variables missing get method.
   *
   * @param $name
   *
   * @return null
   */
  public function __get($name) {
    if (isset($this->{$name})) {
      return $this->{$name};
    }
    return NULL;
  }

  /**
   * Provide magic method for setting variables missing set method.
   *
   * @param $name
   * @param $value
   */
  function __set($name, $value) {
    $this->{$name} = $value;
  }


}
