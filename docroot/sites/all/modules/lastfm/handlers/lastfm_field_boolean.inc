<?php
// $Id;

/**
 * @file
 *   Views field handler for last.fm boolean fields.
 */

/**
 * Views field handler for last.fm boolean fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, otherwise the parent render function is going to
 * fail.
 */
class lastfm_field_boolean extends views_handler_field_boolean {

  /**
   * Called to add the field to a query.
   */
  function query() {  
    $this->field_alias = $this->real_field;  
  }
}
