<?php
namespace entity_context\handler;

/**
 * Handler for User field type.
 */
class User extends \entity_context\core\FieldHandler {
  /**
   * Returns value for field.
   * @return [type] [description]
   */
  public function getValue() {
    $wrapper = $this->getWrapper();
    $context = $this->getContext();

    return $wrapper->{$context['field']}->label();
  }
}