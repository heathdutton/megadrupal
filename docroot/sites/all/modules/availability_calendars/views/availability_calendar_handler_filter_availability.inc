<?php
/**
 * Views handler to filter on availability.
 *
 * This filter inherits from views_handler_filter as inheriting from
 * views_handler_filter_numeric or views_handler_filter_date did not
 * turn out to be a lot easier.
 *
 * This filter allows to filter on availability by accepting the following
 * values:
 * - 1 date (for availability at that given date).
 * - Begin and end date (end date inclusive).
 * - Arrival and departure date (departure date not inclusive).
 * - Start date and duration.
 */
class availability_calendar_handler_filter_availability extends views_handler_filter {
  public static $instance;

  /** @var bool */
  protected $autoSubmit = FALSE;

  public function __construct() {
    self::$instance = $this;
    $this->always_multiple = TRUE;
    module_load_include('inc', 'availability_calendar');
  }

  public function option_definition() {
    $options = parent::option_definition();

    $options['value'] = array(
      'contains' => array(
        'from' => array('default' => ''),
        'to' => array('default' => ''),
        'to1' => array('default' => ''),
        'duration' => array('default' => ''),
      ),
    );
    $options['operator'] = array('default' => 'from_duration');

    return $options;
  }

  public function operators() {
    $operators = array(
      'at' => array(
        'title' => t('At (date)'),
        'method' => 'op_at',
        'summary' => t('at %from'),
        'values' => array('from'),
      ),
      'from_to' => array(
        'title' => t('From begin up to and including end'),
        'method' => 'op_from_to',
        'summary' => t('From %from to %to'),
        'values' => array('from', 'to'),
      ),
      'from_to1' => array(
        'title' => t('From arrival to departure'),
        'method' => 'op_from_to1',
        'summary' => t('From %from to %to1'),
        'values' => array('from', 'to1'),
      ),
      'from_duration' => array(
        'title' => t('From begin during duration'),
        'method' => 'op_from_duration',
        'summary' => t('From %from during %duration days'),
        'values' => array('from', 'duration'),
      ),
    );

    return $operators;
  }

  /**
   * Provides a list of all the availability operators, optionally restricted
   * to only the given property of the operators.
   */
  public function operator_options($which = 'title') {
    $options = array();
    foreach ($this->operators() as $id => $operator) {
      $options[$id] = $operator[$which];
    }
    return $options;
  }

  public function operators_by_value($value) {
    $options = array();
    foreach ($this->operators() as $id => $operator) {
      if (in_array($value, $operator['values'])) {
        $options[] = $id;
      }
    }
    return $options;
  }

  /**
   * Add validation and date popup(s) to the value form.
   */
  public function value_form(&$form, &$form_state) {
    $form['value']['#tree'] = TRUE;
    $form['value']['#attributes']['data-date-format'] = availability_calendar_get_date_format_for_js('AC_DATE_ENTRY');
    $form['value']['#attached']['js'][] = drupal_get_path('module', 'availability_calendar') . '/views/availability_calendar_handler_filter_availability.js';

    if (empty($form_state['exposed'])) {
      // We're in Views edit mode self. Add validator here. When we're in an
      // exposed form, validation will go via exposed_validate().
      $form['value']['#element_validate'][] = 'availability_calendar_handler_filter_availability_validate_value';
    }

    // Determine values to add and their dependencies.
    $dependency_source = NULL;
    if (!empty($form_state['exposed']) && (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id']))) {
      // Exposed form with operator not exposed: only add values for the
      // set operator.
      $operators = $this->operators();
      $values = $operators[$this->operator]['values'];
    }
    else {
      // Views UI.
      $values = array('from', 'to', 'to1', 'duration');
      if (!empty($form['operator'])) {
        $dependency_source = $form['operator']['#type'] === 'radios' ? 'radio:options[operator]' : 'edit-options-operator';
      }
    }

    // Add value fields.
    if (in_array('from', $values)) {
      $form['value']['from'] = $this->value_form_date_field($this->operator, 'from', $dependency_source, $form_state);
    }
    if (in_array('to', $values)) {
      $form['value']['to'] = $this->value_form_date_field($this->operator, 'to', $dependency_source, $form_state);
    }
    if (in_array('to1', $values)) {
      $form['value']['to1'] = $this->value_form_date_field($this->operator, 'to1', $dependency_source, $form_state);
    }
    if (in_array('duration', $values)) {
      $options = array(0 => t('- Select duration -'));
      for($i = 1; $i <= 28; $i++) {
        if ($i % 7 === 0) {
          $options[$i] = format_plural($i / 7, '1 week', '@count weeks');
        }
        else if ($i <= 20) {
          if ($this->definition['allocation_type'] === AC_ALLOCATION_TYPE_FULLDAY) {
            $options[$i] = format_plural($i, '1 day', '@count days');
          }
          else {
            $options[$i] = format_plural($i, '1 night', '@count nights');
          }
        }
      }
      $variable_name = "availability_calendar_views_op_{$this->operator}_duration";
      $form['value']['duration'] = array(
        '#type' => 'select',
        '#title' => availability_calendar_get_customizable_text($variable_name),
    		'#options' => $options,
      	'#default_value' => $this->value['duration'],
      );
      if ($dependency_source !== NULL) {
        $form['value']['duration']['#dependency'] = array($dependency_source => $this->operators_by_value('duration'));
      }
    }
  }

  protected function value_form_date_field($operator, $operand, $dependency_source, $form_state) {
    $variable_name = "availability_calendar_views_op_{$operator}_{$operand}";
    $element = array(
      '#type' => 'textfield',
      '#title' => availability_calendar_get_customizable_text($variable_name),
      '#size' => 12,
      '#default_value' => $this->value[$operand],
    );
    if (!empty($form_state['exposed_form_plugin']->options['autosubmit'])) {
      $form['value']['from']['#attributes'] = array('class' => array('availability-calendar-auto-submit'));
    }
    if (module_exists('date_popup')) {
      $this->change_element_into_date_popup($element, 0);
    }
    else {
      $date_example = new DateTime();
      if ($operand === 'to') {
        $date_example->modify('+6');
      }
      else if ($operand === 'to1') {
        $date_example->modify('+7');
      }
      $date_example = availability_calendar_format_entry_date($date_example);
      $element['#description'] = t('E.g., @date', array('@date' => $date_example));
    }
    if ($dependency_source !== NULL) {
      $element['#dependency'] = array($dependency_source => $this->operators_by_value($operand));
    }

    return $element;
  }
  /**
   * Changes a (text) form element into a date popup element.
   *
   * @param array $element
   * @param int $offset
   */
  protected function change_element_into_date_popup(&$element, $offset) {
    $element['#type'] = 'date_popup';
    $element['#date_label_position'] = 'none';
    $element['#date_type'] = 'DATE_ISO';
    $element['#date_format'] = variable_get('date_format_availability_calendar_date_entry', AC_DATE_ENTRY);
    // @todo: firstDay and maxDate based on field (formatter) settings?
    $element['#datepicker_options'] = array(
      'firstDay' => variable_get('date_first_day', 6),
      'minDate' => $offset,
      'maxDate' => '+24m',
    );
  }

  /**
   * Validates our part of the exposed form.
   *
   * What we accept as valid depends on the situation.
   * Always:
   * - A non empty date or duration must have a valid value.
   * - If this filter is not required, both from and to/to1/duration may be
   *   empty.
   * For non auto-submit forms:
   * - From and to/to1/duration should both be filled in or both be empty.
   * For auto-submit forms:
   * - It may be that only 1 value is filled in. If the input is accepted and
   *   how it is processed depends on what is filled in,
   *   @see accept_exposed_input().
   *
   * Overrides @see views_handler::exposed_validate().
   */
  public function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }
    $this->validate_value($form[$this->options['expose']['identifier']], $form_state);
    // We need this information later on in accept_exposed_input().
    $this->autoSubmit = (bool) !empty($form_state['exposed']) && $form_state['exposed_form_plugin']->options['autosubmit'];
  }

  /**
   * Validate that the values convert to something usable.
   */
  public function validate_value(&$element, $form_state) {
    if (empty($form_state['exposed'])) {
      // In Views UI, the value is required if the filter is not exposed,
      // otherwise we don't validate at all (so people can place "help texts" in
      // the inputs.)
      if ($form_state['values']['options']['expose_button']['checkbox']['checkbox']) {
        return;
      }
      $required = FALSE;
      $autoSubmit = FALSE;
      $operator = $form_state['values']['options']['operator'];
    }
    else {
      // In exposed forms, values are required if "Required" was checked and if
      // this is not an auto-submitted form.
      $required = (bool) $this->options['expose']['required'];
      $autoSubmit = (bool) $form_state['exposed_form_plugin']->options['autosubmit'];
      $operator = empty($this->options['expose']['use_operator']) ? $this->operator : $form_state['values'][$this->options['expose']['operator_id']];
    }
    $operators = $this->operators();
    $values = empty($operator) ? array('from', 'to', 'to1', 'duration') : $operators[$operator]['values'];

    // Set time to midnight as other dates are also set to that time.
    $today = new DateTime();
    $today->setTime(0, 0, 0);

    $value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    $from_valid = FALSE;
    $requireFrom = FALSE;
    if (in_array('from', $values) && array_key_exists('from', $value)) {
      $from_valid = $this->validate_valid_time_1($element['from'], $value['from'], $required && !$autoSubmit, $today, t('Only future availability can be searched.'));
    }
    if (in_array('to', $values) && array_key_exists('to', $value)) {
      $to_valid = $this->validate_valid_time_1($element['to'], $value['to'], ($required || $from_valid instanceof DateTime) && !$autoSubmit, $from_valid, t('The end date should be on or after the start date.'));
      if ($to_valid instanceof DateTime) {
        $requireFrom = TRUE;
      }
    }
    if (in_array('to1', $values) && array_key_exists('to1', $value)) {
      $to1_valid = $this->validate_valid_time_1($element['to1'], $value['to1'], ($required || $from_valid instanceof DateTime) && !$autoSubmit, $from_valid, t('The departure date should be after the arrival date.'));
      if ($to1_valid instanceof DateTime) {
        $requireFrom = TRUE;
      }
    }
    if (in_array('duration', $values) && array_key_exists('duration', $value)) {
      $duration_valid = $this->validate_valid_duration($element['duration'], $value['duration'], ($required || $from_valid instanceof DateTime) && !$autoSubmit);
      if ($duration_valid && !empty($value['duration'])) {
        $requireFrom = TRUE;
      }
    }
    // Do a 2nd check on from based on whether a 2nd operand was filled in,
    // making from required for non auto submitting forms.
    if ($from_valid && $requireFrom && !$autoSubmit && empty($value['from'])) {
      form_error($element['from'], t('Field %field is required.', array('%field' => $element['from']['#title'])));
    }
  }

  /**
   * @param array $element
   * @param array|string $value
   * @param bool $required
   * @param DateTime|bool $minimum
   * @param string $minimum_error_message
   *
   * @return DateTime|bool
   *   A DateTime if the value could be parsed into a Datetime, true if the
   *   value could not be parsed into a valid date but is valid anyway (empty,
   *   default value), false otherwise.
   */
  protected function validate_valid_time_1(&$element, $value, $required, $minimum, $minimum_error_message) {
    // If date popup is enabled, the value will be an array (with a date and
    // time component).
    if (is_array($value)) {
      $value = $value['date'];
    }
    if (empty($value)) {
      $result = !$required;
      if ($required) {
        form_error($element, t('Field %field is required.', array('%field' => $element['#title'])));
      }
    }
    else if (($result = availability_calendar_parse_entry_date($value)) === FALSE) {
      // We accept non valid default values and treat them as empty, so they
      // can be filled with a "help text".
      $result = $value == $element['#default_value'];
      if (!$result) {
        form_error($element, t('Invalid date format.'));
      }
    }
    else if ($minimum instanceof DateTime && $result < $minimum) {
      form_error($element, $minimum_error_message);
      $result = FALSE;
    }
    return $result;
  }

  /**
   * @param array $element
   * @param string|int $value
   * @param bool $required
   *
   * @return bool
   *   Whether the duration field has tobe considered valid.
   */
  protected function validate_valid_duration(&$element, $value, $required) {
    $valid = TRUE;
    if (empty($value)) {
      if ($required) {
        form_error($element, t('Field %field is required.', array('%field' => $element['#title'])));
        $valid = FALSE;
      }
    }
    else if ((!is_int($value) && !ctype_digit($value)) || $value <= 0) {
      form_error($element, t('Duration must be a positive number of days.'));
      $valid = FALSE;
    }
    return $valid;
  }

  /**
   * Check to see if input from the exposed filters should be accepted.
   *
   * For non auto-submit forms:
   * - from and to/to1/duration should both be filled in.
   * For auto-submit forms:
   * - if only from is filled in and to/to1/duration is empty, the duration is
   *   taken as 1; to is taken as from; or to1 is taken as from + 1.
   * - if only to is filled in, from is taken as to
   * - if only to1 is filled in, from is taken as to1 - 1
   * - if only duration is filled in, we validated as correct but do not accept
   *   the exposed input.
   */
  public function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator_id']) && isset($input[$this->options['expose']['operator_id']])) {
      // Fetch operator from form (instead of from $this object)
      $this->operator = $input[$this->options['expose']['operator_id']];
    }

    if (!empty($this->options['expose']['identifier'])) {
      // Fetch values from form (instead of from $this object).
      $this->value = $input[$this->options['expose']['identifier']];

      $operators = $this->operators();
      $values = $operators[$this->operator]['values'];

      // Get valid or emptied value for from.
      $from = &$this->getValidOrEmptyValue($this->value[$values[0]], $values[0]);
      if (count($values) === 1) {
        return !empty($from);
      }
      else {
        // Get valid or emptied value for other value (to/to1/duration).
        $other = &$this->getValidOrEmptyValue($this->value[$values[1]], $values[1]);
        // Try to fill an empty value based on the value of the other operand.
        if ($this->autoSubmit) {
          $this->fillEmptyValueFromOther($from, $other, $values[1]);
        }
        // We can now accept the exposed input if both values are not empty.
        return !empty($from) && !empty($other);
      }

    }
    return TRUE;
  }

  /**
   * Returns the given value if it is valid or an empty value otherwise.
   *
   * @param string|int $value
   * @param string $type
   *
   * @return string|int|null
   *
   */
  protected function &getValidOrEmptyValue(&$value, $type) {
    $empty = empty($value);
    if (!$empty) {
      // If the value is not empty but does not validate, it apparently is
      // the default value ("help text") that should be treated as empty.
      $valid = $type === 'duration'
        ? (is_int($value) || ctype_digit($value)) && $value > 0
        : availability_calendar_parse_entry_date($value) !== FALSE;
      if (!$valid) {
        $value = NULL;
      }
    }
    return $value;
  }

  /**
   * If 1 of the values is empty, try to fill it based on the other value and
   * the (operator) type.
   *
   * @param string|int|null $value1
   * @param string|int|null $value2
   * @param string $type
   */
  protected function fillEmptyValueFromOther(&$value1, &$value2, $type) {
    if ($type === 'to' || $type === 'to1') {
      if (empty($value2)) {
        // Use from value also as to or to1 value, this will also work for to1
        // as this is not corrected by 1 day in op_from_to1().
        $value2 = $value1;
      }
      else if (empty ($value1)) {
        if ($type === 'to') {
          $value1 = $value2;
        }
        else {
          // Correct by 1 day for to1;
          $from = availability_calendar_parse_entry_date($value2);
          $from->modify('-1 day');
          $value1 = $from->format(AC_ISODATE);
        }
      }
    }
  }

  public function query() {
    $operators = $this->operators();
    if (isset($operators[$this->operator]['method'])) {
      $this->{$operators[$this->operator]['method']}();
    }
  }

  protected function op_at() {
    $this->value['duration'] = 1;
    $this->op_from_duration();
  }

  protected function op_from_to() {
    $this->build_query(
      availability_calendar_parse_entry_date($this->value['from']),
      availability_calendar_parse_entry_date($this->value['to']));
  }

  protected function op_from_to1() {
    $from = availability_calendar_parse_entry_date($this->value['from']);
    $to = availability_calendar_parse_entry_date($this->value['to1']);
    if ($from instanceof DateTime && $to instanceof DateTime) {
      // Departure date (to1) is not inclusive. So we modify it by 1 day.
      // But we do accept the same dates for arrival (from) and departure (to1).
      // In that case we leave the to date as is (equal to the from date).
      if ($to > $from) {
        $to->modify('-1 day');
      }
      $this->build_query($from, $to);
    }
  }

  protected function op_from_duration() {
    $this->build_query(availability_calendar_parse_entry_date($this->value['from']), (int) $this->value['duration']);
  }

  /**
   * Helper method for the op_... methods that builds the query.
   *
   * @param DateTime $from
   * @param DateTime|integer $to_or_duration
   */
  protected function build_query($from, $to_or_duration) {
    $this->ensure_my_table();
    availability_calendar_query_available($this->query,
      $this->table_alias,
      $this->real_field,
      $from,
      $to_or_duration,
      $this->definition['default_state']);
  }

  public function admin_summary() {
    $output = '';
    if (!empty($this->options['exposed'])) {
      $output = t('exposed');
    }
    else {
      $operators = $this->operators();
      if (isset($operators[$this->operator]['summary'])) {
        $arguments = array();
        foreach($this->value as $key => $value) {
          $arguments["%$key"] = $value;
        }
        $output = format_string($operators[$this->operator]['summary'], $arguments);
      }
    }
    return $output;
  }
}

/**
 * Form element validator for the date field(s): forward to the
 * availability_calendar_handler_filter_availability::validate_value()
 * method inside the class.
 */
function availability_calendar_handler_filter_availability_validate_value(&$element, &$form_state, $form) {
  availability_calendar_handler_filter_availability::$instance->validate_value($element, $form_state);
}
