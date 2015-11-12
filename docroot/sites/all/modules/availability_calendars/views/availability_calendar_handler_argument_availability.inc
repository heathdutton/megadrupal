<?php
/**
 * Defines a contextual filter to filter on availability.
 */
class availability_calendar_handler_argument_availability extends views_handler_argument_date {
  /** @var date_sql_handler Only used if date api module is enabled. */
  protected $date_handler;
  /** @var string[] */
  protected $date_parts;
  /** @var DateTime */
  protected $min_date;
  /** @var DateTime */
  protected $max_date;

  public function init(&$view, &$options) {
    parent::init($view, $options);

    // Add a date handler.
    if (module_exists('date_api')) {
      module_load_include('inc', 'date_api', 'date_api_sql');
      $this->date_handler = new date_sql_handler(DATE_UNIX);
    }
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['end_date_exclusive'] = array('default' => $this->definition['allocation_type'] == AC_ALLOCATION_TYPE_OVERNIGHT, 'bool' => TRUE);
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['end_date_exclusive'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude end date'),
      '#description' => t('If selected, the end date itself will not be taken into account when filtering on availability, typical for overnight rental. This setting will be ignored if only a single date is passed in.'),
      '#default_value' => !empty($this->options['end_date_exclusive']),
    );
  }

  /**
   * Inject a test for valid date range before the regular query.
   * Override the parent query to be able to control the $group.
   */
  public function query($group_by = FALSE) {
    if (empty($this->argument)) {
      return;
    }

    module_load_include('inc', 'availability_calendar');

    if (module_exists('date_api')) {
      $this->date_parts = $this->date_handler->arg_parts($this->argument);
      $this->min_date = new DateTime($this->date_handler->complete_date($this->date_parts[0]['date'], 'min'));
      $this->max_date = count($this->date_parts) > 1 ? new DateTime($this->date_handler->complete_date($this->date_parts[1]['date'], 'min')) : clone $this->min_date;
    }
    else {
      $this->date_parts = explode('--', $this->argument);
      $this->min_date = availability_calendar_parse_iso_date($this->date_parts[0]);
      $this->max_date = count($this->date_parts) > 1 ? availability_calendar_parse_iso_date($this->date_parts[1]) : clone $this->min_date;
    }

    // Adjust end date if it is not inclusive and differs from the start date.
    if (!empty($this->options['end_date_exclusive']) && $this->min_date < $this->max_date) {
      $this->max_date->modify('-1 day');
    }

    $this->build_query();
  }

  /**
   * Helper method for the query method that actually builds the query.
   * Extracted to allow it to be overridden.
   */
  protected function build_query() {
    $this->ensure_my_table();
    availability_calendar_query_available($this->query,
      $this->table_alias,
      $this->real_field,
      $this->min_date,
      $this->max_date,
      $this->definition['default_state']);
  }

  public function get_sort_name() {
    return t('Numeric', array(), array('context' => 'Sort order'));
  }
}
