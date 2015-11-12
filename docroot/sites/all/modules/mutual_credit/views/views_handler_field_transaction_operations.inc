<?php
// $Id:

/**
 * @file
 * Field handler to show all the permitted transaction operations for a transaction
 * this should always be defined with
 *
 * additional fields => array('serial')
 */
class views_handler_field_transaction_operations extends views_handler_field_transaction {

  function render($values) {
    $renderable = transaction_get_links(
      transaction_load($values->{$this->aliases['serial']}),
      FALSE, 
      TRUE
    );
    return drupal_render($renderable);
  }

}
