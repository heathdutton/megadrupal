<?php
/**
 * @file
 * Contains the abstract class used by all handlers.
 */

/**
 * Class Qui_Query_Builder_Handler
 */
abstract class Qui_Query_Builder_Handler {

  protected $controller;
  protected $elements;
  public $defaults;
  protected $values;
  protected $action;
  protected $name;

  const WEIGHT_DELTA = 50;

  public $enabled = FALSE;

  /**
   * Sets variables that point to the controller class.
   *
   * @param Qui_Query_Builder_Controller $controller
   *   Controller.
   */
  public function __construct(Qui_Query_Builder_Controller $controller) {
    $this->controller = &$controller;
    $this->defaults = array();
    $this->report = &$this->controller->report;
    $this->action = $this->controller->action;
  }

  /**
   * Gets the next highest column weight.
   */
  protected function nextHighestWeight() {
    $next_weight = 0;
    foreach ($this->defaults as $item) {
      if ($next_weight <= $item['weight']) {
        $next_weight = $item['weight'] + 1;
      }
    }
    return ($next_weight <= self::WEIGHT_DELTA) ? $next_weight : self::WEIGHT_DELTA;
  }

  /**
   * Sort an array of query types.
   *
   * @return array
   *   Array of values sorted by weight.
   */
  protected function sortByWeight() {
    $cloned = $this->defaults;
    usort($cloned, '_qui_weight_sort');
    return $cloned;
  }

  /**
   * Gets default values from the form state or the report.
   *
   * @param $key
   * @return mixed
   */
  protected function getDefaultValues($key) {
    if (isset($this->controller->form_state['values'])) {
      // Info needs to come from the form_state.
      return $this->formValue($key);
    }
    if (isset($this->report)) {
      // Info can come from the predefined QuiReport.
      return $this->reportValue($key, 'query');
    }
  }

  /**
   * Gets a value from the form state.
   *
   * This might be a bit of a hack. But it does make it easy to return items
   * without having to worry about isset checks.
   *
   * @param string $key
   *   Key to find in the array.
   * @param string $subs,...
   *   Variable number of sub parts to search into.
   */
  public function formValue($key, $subs = NULL) {
    $args = array_slice(func_get_args(), 1);
    $args = array_map(function ($v) {
      return "'" . $v . "'";
    }, $args);
    $form_state_array = '$this->controller->form_state[\'values\']';

    if ($args) {
      $form_state_array .= '[' . implode('][', $args) . ']';
    }
    $form_state_array .= "['{$key}']";
    return eval("return isset($form_state_array) ? $form_state_array : array();");
  }

  /**
   * Gets a value from the report object
   *
   * @param string $key
   *   Key to find in the array.
   * @param string $subs,...
   *   Variable number of sub parts to search into.
   */
  public function reportValue($key, $subs = NULL) {
    $args = array_slice(func_get_args(), 1);
    $report_object = '$this->controller->report';

    if ($args) {
      $report_object .= '->' . implode('->', $args);
    }
    $report_object .= "->{$key}";
    return eval("return isset($report_object) ? $report_object : array();");
  }

  public function setDefaults() {

  }

  /**
   * Gets a handler object from the controller.
   *
   * @param string $handler
   *   Handler key.
   * @return mixed
   *   Handler object.
   */
  protected function getHandler($handler) {
    return $this->controller->getHandler($handler);
  }

  /**
   * Combines form elements for rendering.
   */
  public function render() {
    if (is_array($this->elements)) {
      $this->controller->form = array_merge_recursive($this->controller->form, $this->elements);
    }
  }

  /**
   * Triggers handler method based on clicked button action.
   */
  public function executeButtonAction() {
    if (isset($this->controller->form_state['clicked_button']['#button_action'])) {
      $function = $this->controller->form_state['clicked_button']['#button_action'];
      if (method_exists($this, $function)) {
        $this->$function();
      }
    }
  }

  /**
   * Method called by controller.
   */
  public function build() {

  }

  /**
   * Method called by controller.
   */
  public function preBuild() {

  }

  /**
   * Create a help link that points to Drupal.org documentation.
   *
   * @param string $url
   *   URL to the help page.
   * @param $description
   *   Description to show on hover.
   *
   * @return string
   *   Help link.
   */
  protected function helpLink($url, $description) {
    return l(t('Need Help?'), $url, array(
      'attributes' => array(
        'title' => $description,
        'target' => '_blank',
        'class' => 'qui-help-link',
      ),
    ));
  }

  /**
   * Formats a table name.
   *
   * @param string $table
   *   Table name.
   *
   * @return string
   *   Formatted table name.
   */
  protected function formatTableName($table) {
    $display = $table;
    if (strpos($display, 'field_data_') !== FALSE) {
      // Reformat for better display.
      $display = str_replace('field_data_', '', $display);
    }
    $display = str_replace('_', ' ', $display);
    $display = ucwords($display);
    return $display;
  }

  /**
   * Gets columns for a set of tables.
   *
   * @param array $tables
   *   Tables array.
   *
   * @return array
   *   Array of columns keyed by table name.
   */
  protected function getColumns(array $tables = array()) {
    foreach ($tables as $table) {
      $columns[$table] = $this->getTableColumns($table);
    }
    ksort($columns);
    return $columns;
  }

  /**
   * Get columns with the display formatted.
   *
   * @param array $tables
   *   Tables array.
   *
   * @return array
   *   Array of columns with names formatted.
   */
  protected function getColumnsFormatted(array $tables = array()) {
    $return = array();
    foreach ($this->getColumns($tables) as $table => $columns) {
      foreach ($columns as $key => $column) {
        $return[$table][$key] = $this->formatTableName($table) . " ({$column})";
      }
    }
    return $return;
  }

  /**
   * Gets an array of columns with the column as the key.
   *
   * @param string $table
   *   Table name.
   *
   * @return array
   *   Array of columns.
   */
  protected function getColumnsNoTable($table) {
    $columns = $this->getTableColumns($table);
    return array_combine(array_values($columns), array_values($columns));
  }

  /**
   * Gets active columns based on tables in the form_state.
   *
   * @return array
   *   Array of columns.
   */
  protected function getActiveColumns() {
    return $this->getColumns($this->getActiveTables());
  }

  /**
   * Gets a table list for the current database.
   *
   * @return array
   *   Array of tables.
   */
  protected function getTables() {
    $db = $this->getHandler('qui_database')->defaults;
    db_set_active($db);
    $conn = Database::getConnection();
    $db_info = $conn->getConnectionOptions();
    $prefix = $db_info['prefix'][$db];
    $tables = db_find_tables($prefix . '%');
    db_set_active();
    if ($prefix) {
      // Replace the prefix string in the table names.
      $tables = array_map(function ($table_name) use ($prefix) {
        return str_replace($prefix, '', $table_name);
      }, $tables);
      $tables = array_combine($tables, $tables);
    }
    return array_filter($tables, '_qui_filter_tables');
  }

  /**
   * Gets formatted columns from active tables only.
   *
   * @return array
   *   Array of columns.
   */
  protected function getFormattedActiveColumns() {
    return $this->getColumnsFormatted($this->getActiveTables());
  }

  /**
   * Gets active tables from the form_state.
   *
   * @return array
   *   Array of tables.
   */
  protected function getActiveTables() {
    $tables[] = $this->getHandler('qui_select_table')->defaults;
    foreach ((array) $this->getHandler('qui_joins')->defaults as $join) {
      $tables[] = $join['table'];
    }
    return array_filter($tables);
  }

  /**
   * Gets columns for a specific table keyed by TABLE.COLUMN.
   *
   * @param string $table
   *   Table name.
   *
   * @return array
   *   Array of columns.
   */
  protected function getTableColumns($table) {
    $columns = &drupal_static(__FUNCTION__ . '_' . $table, array());
    if ($columns) {
      return $columns;
    }
    db_set_active($this->getHandler('qui_database')->defaults);
    $conn = Database::getConnection();
    $db_info = $conn->getConnectionOptions();
    $results = db_query("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = :db AND TABLE_NAME = :table", array(
      ':table' => $table,
      ':db' => $db_info['database'],
    ))->fetchAllKeyed(0, 0);
    db_set_active();
    foreach ($results as $column) {
      $columns[$table . '.' . $column] = $column;
    }
    ksort($columns);
    return $columns;
  }

  /**
   * Gets the clicked button from the form_state.
   *
   * @return array
   *   Clicked button array.
   */
  protected function clickedButton() {
    if (isset($this->controller->form_state['clicked_button'])) {
      return $this->controller->form_state['clicked_button'];
    }
    return array();
  }

  /**
   * Gets the button action from the clicked button.
   *
   * @return string
   *   Button action.
   */
  protected function buttonAction() {
    $clicked_button = $this->clickedButton();
    if (isset($clicked_button['#button_action'])) {
      return $clicked_button['#button_action'];
    }
    else {
      return '';
    }
  }

  /**
   * Gets the button name from the clicked button.
   *
   * @return string
   *   Button name.
   */
  protected function buttonName() {
    $clicked_button = $this->clickedButton();
    if (isset($clicked_button['#name'])) {
      return $clicked_button['#name'];
    }
    else {
      return '';
    }
  }

  /**
   * Gets the default ajax settings used for form elements.
   *
   * @return array
   *   AJAX settings array.
   */
  protected function ajaxSettings() {
    return array(
      'callback' => '_qui_query_builder_ajax_callback',
      'wrapper' => 'qui-create-report-form',
    );
  }

  /**
   * Method called by controller.
   */
  public function validate() {

  }

  /**
   * Method called by controller.
   */
  public function submit() {

  }

}
