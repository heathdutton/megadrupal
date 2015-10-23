<?php
namespace entity_context\handler;

/**
 * Handler for Text Formatted field type.
 */
class TextFormatted extends \entity_context\core\FieldHandler {
  /**
   * Returns value for field.
   * @return [type] [description]
   */
  public function getValue() {
    $wrapper = $this->getWrapper();
    $context = $this->getContext();

    $value = $wrapper->{$context['field']}->value->value(array('sanitize' => TRUE));
    $value = trim(check_plain(strip_tags($value)));

    return $value;
  }
}