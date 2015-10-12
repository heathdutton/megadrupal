<?php

/**
 * Creates v in code.
 *
 * @code
 * $my_variables = CINC::init('Variables');
 * @endcode
 */
class CINC_Variables extends CINC implements CINC_CRUD {

  public $cinc_yaml_prefix = 'variables';

  function __construct() {
    $this->config = array();
  }

  /**
   * Saves variables to the database.
   */
  public function create() {
    foreach ($this->config as $variable => $value) {
      variable_set($variable, $value);
    }
    return $this;
  }

  /**
   * Tests variables exists.
   * Variables always exists.
   */
  public function exists() {
    return TRUE;
  }

  /**
   * Loads variables from the database.
   */
  public function read() {
    $this->config = variable_initialize();
    return $this;
  }

  /**
   * Loads list of machine names.
   */
  static public function read_list() {
    $list = array();
    return $list;
  }

  /**
   * Saves variables to the database.
   */
  public function update() {
    $this->create();
    return $this;
  }

  /**
   * Removes variables from the database.
   */
  public function delete() {
    global $conf;

    $names = array_keys($this->config);

    db_delete('variable')
      ->condition('name', $names, 'IN')
      ->execute();
    cache_clear_all('variables', 'cache_bootstrap');

    foreach ($names as $name) {
      unset($conf[$name]);
    }
    return $this;
  }


}
