<?php
namespace entity_context\handler;

/**
 * Handler for Boolean field type.
 */
class Boolean extends Text {
  /**
   * Returns value for field.
   * @return [type] [description]
   */
  public function getValue() {
    $value =  parent::getValue();
    $value = $value ? 'yes' : 'no'; // don't run through t();

    return $value;
  }
}