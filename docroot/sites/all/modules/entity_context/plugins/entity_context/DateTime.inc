<?php
namespace entity_context\handler;

/**
 * Handler for Datetime field type.
 */
class DateTime extends Text {
  /**
   * Returns value for field.
   * @return [type] [description]
   */
  public function getValue() {
    return format_date(parent::getValue());
  }
}