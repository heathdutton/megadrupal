<?php
/**
 * @file
 * Field handler to provide acess control for the encrypted email field
 */

/**
 * A handler to display the uncrypted email version.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_dbee_user_mail extends views_handler_field_user_mail {

  function render($values) {
    $values->{$this->field_alias} = dbee_decrypt($values->{$this->field_alias});
    // In all cases, go throught the parent default way
    return parent::render($values);
  }
}
