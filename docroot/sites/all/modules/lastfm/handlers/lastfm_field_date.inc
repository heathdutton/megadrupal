<?php
// $Id;

/**
 * @file
 *   Views field handler for last.fm date fields.
 */

/**
 * Views field handler for last.fm date fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, otherwise the parent render function is going to
 * fail.
 */
class lastfm_field_date extends views_handler_field_date {

  /**
   * Called to add the field to a query.
   */
  function query() {  
    $this->field_alias = $this->real_field;
  }
}
