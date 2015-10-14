<?php
/**
 * Defines a field handler for a sql date type.
 */
class availability_calendar_handler_field_sql_date extends views_handler_field_date {
  /**
   * Set our "own" date display format as default.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['date_format'] = array('default' => 'availability_calendar_date_display');
    return $options;
  }

  /**
   * Overrides the parent method to unset the timezone field.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['timezone']);
  }

  /**
   * Overrides parent method to convert the value from a ISO date to a
   * timestamp, so the render() method in the base class will work as expected.
   */
  public function get_value($values, $field = NULL) {
    $value = parent::get_value($values, $field);
    if (!empty($value)) {
      $value = new DateTime($value);
      if (in_array($this->options['date_format'], array('raw time ago', 'time ago', 'raw time hence', 'time hence', 'raw time span', 'inverse time span', 'time span'))) {
        // Render method will display difference with REQUEST_TIME: set time parts to equal that so the difference will be whole days.
        $request_time = new DateTime(REQUEST_TIME);
        $value->setTime((int) $request_time->format('G'), (int) $request_time->format('i'), (int) $request_time->format('s'));
      }
      else {
        // Render method will display absolute date: cut off any time parts.
        $value->setTime(0, 0, 0);
      }

      //PHP5.3: $value = $value->getTimestamp();
      $value = $value->format('U');
    }
    return $value;
  }
}
