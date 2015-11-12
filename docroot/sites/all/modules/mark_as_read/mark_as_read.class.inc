<?php

/**
 * @file
 * This file contains the general classes.
 */

/**
 * This class represents the individual list DB object.
 */
class MarkAsReadListORM {

  public $isNew = TRUE;

  /**
   * Construct the object based on the argument provided.
   *
   * @param string $list_id
   *   Individual List Id.
   */
  public function __construct($list_id = NULL) {
    if ($list_id) {
      $this->isNew = FALSE;
      $this->load($list_id);
    }
  }

  /**
   * Write data if property is invalid/inaccessible.
   *
   * @param string $property
   *   Inaccessible property.
   * @param string $value
   *   Inaccessible value of the property.
   */
  public function __set($property, $value) {
    $this->{$property} = $value;
  }

  /**
   * Read data from inaccessible properties.
   *
   * @param string $property
   *   Inaccessible Property.
   *
   * @return mixed
   *   Returns the value of the class property.
   */
  public function __get($property) {
    if (property_exists($this, $property)) {
      return $this->{$property};
    }
    return NULL;
  }

  /**
   * Load the list from Database.
   *
   * @param int $list_id
   *   Id of the Individual List.
   */
  protected function load($list_id) {
    $list = mark_as_read_db_get_list_by_id($list_id);
    foreach ($list as $property => $value) {
      $this->{$property} = $value;
    }
  }

  /**
   * Saves to the DB.
   */
  public function save() {
    if ($this->isNew) {
      return mark_as_read_insert_list_details($this->list_name, $this->css_selector, $this->attribute_name);
    }
    else {
      return mark_as_read_update_list_details($this->list_id, $this->list_name, $this->css_selector, $this->attribute_name);
    }
  }
}
