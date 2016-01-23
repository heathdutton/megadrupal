<?php
/**
 *  @file
 * Adds a field as a general field. Markup fields use this handler.
 */

/**
 * Handler for a subject.
 */
class odata_handler_field extends views_handler_field {

  function construct() {
    parent::construct();
  }

  /**
  * Called to add the field to a query.
  */
  function query() {
    $this->field_alias = $this->real_field;
    $this->query->select_fields[] = $this->real_field;
  }

  
}
