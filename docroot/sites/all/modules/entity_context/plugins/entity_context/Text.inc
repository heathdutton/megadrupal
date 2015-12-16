<?php
namespace entity_context\handler;

/**
 * Handler for Text field type.
 */
class Text extends \entity_context\core\FieldHandler {
  /**
   * Returns value for field.
   * @return [type] [description]
   */
  public function getValue() {
    $wrapper = $this->getWrapper();
    $context = $this->getContext();

    return $wrapper->{$context['field']}->value(array('sanitize' => TRUE));
  }
}