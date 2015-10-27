<?php
/**
 * @file
 * Entity classes for DQUI Reports.
 */

/**
 * Class QuiReport.
 */
class QuiReport extends Entity {

  public $id;
  public $name;
  public $machine_name;
  public $query;
  public $settings;
  public $description;
  public $tags;
  public $created;
  public $changed;
  public $module;
  public $author_uid;
  public $status;

  /**
   * Creates the QuiReport Entity.
   *
   * @param array $values
   *   Array of QuiReport settings.
   *
   * @see Entity::__construct()
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'qui_report');
    if (!is_array($this->tags)) {
      $this->tags = array_map('trim', explode(',', $this->tags));
    }
    if (is_string($this->query)) {
      $this->query = (object) drupal_json_decode($this->query);
    }
    if (is_string($this->settings)) {
      $this->settings = (object) drupal_json_decode($this->settings);
    }
    if (empty($this->query)) {
      $this->query = new stdClass();
    }
    if (empty($this->settings)) {
      $this->settings = new stdClass();
    }
    return $this;
  }

  /**
   * Formats a QuiReport for saving.
   *
   * @see Entity::save()
   */
  public function save() {
    if (!$this->created) {
      $this->created = REQUEST_TIME;
    }
    if (!$this->author_uid) {
      global $user;
      $this->author_uid = $user->uid;
    }
    if (is_array($this->tags)) {
      $this->tags = implode(', ', $this->tags);
    }
    $this->changed = REQUEST_TIME;
    $this->settings = drupal_json_encode($this->settings);
    $this->query = drupal_json_encode($this->query);
    return parent::save();
  }

  /**
   * Gets the default uri for the QuiReport.
   *
   * @return array
   *   URI.
   */
  public function defaultUri() {
    return array('path' => 'admin/reports/qui/manage/' . $this->identifier() . '/view');
  }

  /**
   * Builds a db_select() dynamic query based on a report query.
   *
   * @return array
   *   Array of query results.
   */
  public function runQuery() {
    $query = $this->buildQuery();
    $results = & drupal_static(__FUNCTION__ . '_' . md5(serialize($this->query)), array());
    if ($results) {
      return $results;
    }
    // Return row count only.
    if ($this->query->row_count) {
      $results = $query->execute()->rowCount();
    }
    else {
      // Return the normal result set.
      $results = $query->execute()->fetchAll();
    }
    return $results;
  }

  /**
   * Builds the query for a report. Does not execute.
   *
   * @return SelectQuery
   *   Drupal SelectQuery object.
   */
  public function buildQuery() {
    // Set the default DB to use.
    if ($this->query->database !== 'default') {
      db_set_active($this->query->database);
    }
    // Set the base table to select from.
    $query = db_select($this->query->select_table);

    // Set the database back to default.
    db_set_active();

    // Add columns.
    if (array_filter((array) $this->query->columns)) {
      // Add each field separately.
      foreach ($this->query->columns as $field) {
        $parts = explode('.', $field['column']);
        $query->addField($parts[0], $parts[1], $field['alias']);
      }
    }
    elseif (empty($this->query->columns)) {
      $query->fields($this->query->select_table);
    }

    // Add joins.
    usort($this->query->joins, '_qui_weight_sort');
    foreach ((array) $this->query->joins as $join) {
      switch ($join['type']) {
        case 'left':
          $query->leftJoin($join['table'], NULL, $join['left_col'] . '=' . $join['right_col']);
          break;

        case 'right':
          $query->rightJoin($join['table'], NULL, $join['left_col'] . '=' . $join['right_col']);
          break;

        default:
          // Cover the default join (which is an inner join).
          $query->join($join['table'], NULL, $join['left_col'] . '=' . $join['right_col']);
      }
    }

    // Add conditions.
    usort($this->query->conditions, '_qui_weight_sort');
    foreach ((array) $this->query->conditions as $c) {
      $value = _qui_format_get_param($c['value']);

      $op = _qui_get_conditions();

      // Special handling for subselects.
      if (strpos($c['value'], 'report:') === 0) {
        $parts = explode(':', $c['value']);
        $subselect = qui_report_load_by_machine_name(trim($parts[1]))->buildQuery();
        $query->condition($c['column'], $subselect, $op[$c['operator']]);
        continue;
      }

      // Special handling for certain operations.
      if (isset($c['operator'])) {
        switch ($c['operator']) {
          case 'is_null':
            $query->isNull($c['column']);
            continue 2;

          case 'is_not_null':
            $query->isNotNull($c['column']);
            continue 2;

          case 'in':
          case 'not_in':
            $value = array_map('trim', explode(',', $value));
            $query->condition($c['column'], $value, $op[$c['operator']]);
            continue 2;
        }
      }

      // Execute the condition depending on the type.
      switch ($c['type']) {
        case 'where':
          $query->condition($c['column'], $c['value'], $op[$c['operator']]);
          break;

        case 'having':
          $query->havingCondition($c['column'], $c['value'], $op[$c['operator']]);
          break;

        case 'where_snippet':
          $query->where($c['value']);
          break;

        case 'having_snippet':
          $query->having($c['value']);
          break;
      }

    }

    // Add expressions.
    usort($this->query->expressions, '_qui_weight_sort');
    foreach ((array) $this->query->expressions as $exp) {
      $query->addExpression($exp['value'], $exp['alias']);
    }

    // Add group by column.
    foreach ($this->query->group as $group) {
      $query->groupBy($group['column']);
    }

    // Add order by column.
    usort($this->query->order, '_qui_weight_sort');
    foreach ($this->query->order as $order) {
      $query->orderBy($order['column'], $order['direction']);
    }

    // Add distinct.
    if ($this->query->distinct) {
      $query->distinct();
    }

    // Apply range.
    if ($this->query->range['min'] || $this->query->range['max']) {
      if ($this->query->range['max']) {
        // Need to set the length parameter as max - min.
        $query->range($this->query->range['min'], $this->query->range['max'] - $this->query->range['min']);
      }
      else {
        // No length parameter required.
        $query->range($this->query->range['min']);
      }
    }

    // Allow any module to alter to this query.
    drupal_alter('qui_report_query', $query, $this);
    drupal_alter('qui_report_' . $this->machine_name . '_query', $query, $this);

    return $query;
  }

  /**
   * Magic method to return a report as a string.
   *
   * Includes curly braces rather than prefixed table names.
   *
   * @return string
   *   A query string.
   */
  public function __toString() {
    $query = $this->buildQuery();
    $query->preExecute();
    $sql = (string) $query;
    $quoted = array();
    if ($this->query->database !== 'default') {
      db_set_active($this->query->database);
    }
    $connection = Database::getConnection();
    foreach ((array) $query->arguments() as $key => $val) {
      $quoted[$key] = $connection->quote($val);
    }
    if (!empty($quoted)) {
      $sql = strtr($sql, $quoted);
    }
    db_set_active();

    return $sql;
  }

  /**
   * Creates the exact query that will be executed.
   *
   * Complete with curly braces replaced with the actual prefixes (or removed).
   *
   * @return string
   *   A query string.
   */
  public function queryString() {
    $sql = $this->__toString();
    if ($this->query->database !== 'default') {
      db_set_active($this->query->database);
    }
    $connection = Database::getConnection();
    $sql = $connection->prefixTables($sql);
    db_set_active();
    return $sql;
  }
}

/**
 * Class QuiReportController.
 */
class QuiReportController extends EntityAPIControllerExportable {

  /**
   * Gets the QuiReport output formatted.
   *
   * @see EntityAPIController::buildContent()
   */
  public function buildContent($entity, $view_mode = 'default', $langcode = NULL, $content = array()) {
    $view_mode = $view_mode == 'default' ? 'table' : $view_mode;
    $content['formatted'] = array(
      '#markup' => qui_get_formatted_report($entity, $view_mode),
    );
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }

}
