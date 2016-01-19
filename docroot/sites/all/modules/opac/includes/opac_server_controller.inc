<?php

/**
 * @file
 * Contains OpacServerController.
 */

class OpacServerController extends EntityAPIController {

  /**
   * Override parent::create.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return array
   *   A new instance of the entity
   */
  public function create(array $values = array()) {
    $values += array(
      'serv_name' => '',
    );
    return parent::create($values);
  }
}
