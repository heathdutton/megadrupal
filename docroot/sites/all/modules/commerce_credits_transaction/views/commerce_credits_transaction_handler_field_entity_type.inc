<?php
/**
 * @file
 * Commerce Credits Transaction Handler Field Entity TYpe
 */


/**
 * Display the entity type with its proper name.
 */
class commerce_credits_transaction_handler_field_entity_type extends views_handler_field {

  public function render($values) {
    $value = $this->get_value($values);

    $info = entity_get_info($value);

    return $info['label'];
  }

}
