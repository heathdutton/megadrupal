<?php

/**
 * @file
 * Smartling controller.
 */

/**
 * SmartlingInterfaceEntity class.
 */
class SmartlingInterfaceEntity extends Entity {

  /**
   * Get default label.
   *
   * @return string
   *   Return label.
   */
  protected function defaultLabel() {
    return $this->title;
  }

  /**
   * Get default uri string for smartling interface entity.
   *
   * @return string
   *   Return default uri string.
   */
  protected function defaultUri() {
    return array('path' => 'admin/config/regional/smartling/interface-entity-data/' . $this->identifier());
  }

}

/**
 * SmartlingInterfaceEntityController class.
 */
class SmartlingInterfaceEntityController extends EntityAPIController {

  /**
   * Create smartling interface entity object.
   *
   * @param array $values
   *   Entity values.
   *
   * @return object
   *   Return smartling interface entity object.
   */
  public function create(array $values = array()) {
    $values += array(
      'gid' => '',
      'textgroup' => '',
      'textgroup_name' => '',
    );

    return parent::create($values);
  }

}
