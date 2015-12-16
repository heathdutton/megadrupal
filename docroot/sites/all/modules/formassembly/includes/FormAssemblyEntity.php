<?php

/**
 * @file
 * An Entity class for the FormAssembly entity
 *
 * Longer file description goes here
 * Author: Shawn P. Duncan
 * Date: 7/23/14
 * Time: 1:14 PM
 */
class FormAssemblyEntity extends Entity {

  /**
   * Overrides the parent method to implement a custom default label.
   *
   * @return string
   *   Returns the name property of the fa_form
   */
  protected function defaultLabel() {
    return $this->name;
  }

  /**
   * Overrides the parent method to implement a custom default URI.
   *
   * @return array
   *   Drupal path specification
   */
  protected function defaultUri() {
    return array('path' => 'formassembly/' . $this->identifier());
  }

  /**
   * Get the value on the fa_form->modified field.
   *
   * @return string
   *   Unix timestamp as Drupal date
   */
  public function getModified() {
    return $this->modified;
  }


  /**
   * Fully update the entity with new data obtained from FormAssembly.
   *
   * The Form Assembly ID (faid) is persistent.
   *
   * @param array $data
   *   An array of values to set
   */
  public function updateData($data) {
    $this->modified = $data['modified'];
    $this->name = $data['name'];
  }

}
