<?php
/**
 * Defines a filter handler for sql date types.
 *
 * @todo: plugin date_popup
 */
class availability_calendar_handler_filter_sql_date extends views_handler_filter_date {
  /**
   * Overrides parent method to change the help on the type selector.
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    if (isset($form['value']['type']['#options'])) {
      $form['value']['type']['#options']['date'] = t('A date in any machine readable format. CCYY-MM-DD is preferred.');
      $form['value']['type']['#options']['offset'] = t('An offset from the current date such as "!example1" or "!example2"', array('!example1' => '+1 day', '!example2' => '-2 weeks'));
    }
  }

  /**
   * Overrides the parent method to use date only arithmetic and to format the
   * values as ISO dates as accepted and expected by the databases.
   */
  public function op_between($field) {
    if ($this->value['type'] === 'offset') {
      // Dates relative to now.
      $from = new DateTime('@' . REQUEST_TIME);
      $from->modify($this->value['min']);
      $to = new DateTime('@' . REQUEST_TIME);
      $to->modify($this->value['max']);
    }
    else {
      // Absolute dates.
      $from = new DateTime($this->value['min']);
      $to = new DateTime($this->value['max']);
    }

    // Cut off any time parts and format as ISO date.
    $from->setTime(0, 0, 0);
    $from = $from->format(AC_ISODATE);
    $to->setTime(0, 0, 0);
    $to = $to->format(AC_ISODATE);

    $this->query->add_where_expression($this->options['group'], "$field $this->operator '$from' AND '$to'");
  }

  /**
   * Overrides the parent method to use date only arithmetic and to format the
   * value as an ISO date as accepted and expected by the databases.
   */
  public function op_simple($field) {
    if ($this->value['type'] === 'offset') {
      // Date relative to now.
      $value = new DateTime('@' . REQUEST_TIME);
      $value->modify($this->value['value']);
    }
    else {
      // Absolute date.
      $value = new DateTime($this->value['min']);
    }

    // Cut off any time parts and format as ISO date.
    $value->setTime(0, 0, 0);
    $value = $value->format(AC_ISODATE);

    $this->query->add_where_expression($this->options['group'], "$field $this->operator '$value'");
  }
}
