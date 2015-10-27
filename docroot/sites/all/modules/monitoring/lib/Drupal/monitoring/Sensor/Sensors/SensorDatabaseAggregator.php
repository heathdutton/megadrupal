<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorDatabaseAggregator.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorExtendedInfoInterface;
use Drupal\monitoring\Sensor\SensorThresholds;

/**
 * Base for database aggregator sensors.
 *
 * Provides basic query build logic and generic verbosity.
 *
 * Defines sensor settings:
 * - table: Name of the table to query on.
 * - conditions: A list of conditions to apply to the query.
 *   - field: Name of the field to filter on. Configurable fields are supported
 *     using the field_name.column_name syntax.
 *   - value: The value to limit by, either an array or a scalar value.
 *   - operator: Any of the supported operators.
 * - time_interval_field: Timestamp field name
 * - time_interval_value: Number of seconds defining the period
 *
 * Adds time interval to sensor settings form.
 */
class SensorDatabaseAggregator extends SensorThresholds implements SensorExtendedInfoInterface {

  /**
   * The result of the db query execution.
   *
   * @var \DatabaseStatementInterface
   */
  protected $queryResult;

  /**
   * The fetched object from the query result.
   *
   * @var object
   */
  protected $fetchedObject;

  /**
   * The arguments of the executed query.
   *
   * @var array
   */
  protected $queryArguments;

  /**
   * {@inheritdoc}
   */
  public function settingsForm($form, &$form_state) {
    $form = parent::settingsForm($form, $form_state);

    // Allow to configure the time interval if a time interval field is
    // configured.
    if ($this->info->getSetting('time_interval_field')) {
      $form['time_interval_value'] = array(
        '#type' => 'select',
        '#title' => t('Aggregate time interval'),
        '#options' => $this->getTimeIntervalOptions(),
        '#description' => t('Select the time interval for which the results will be aggregated.'),
        '#default_value' => $this->info->getTimeIntervalValue(),
      );
    }

    return $form;
  }

  /**
   * Executes the query and returns the result.
   *
   * @return \DatabaseStatementInterface
   *   The query result.
   */
  protected function getQueryResult() {
    if ($this->queryResult === NULL) {
      $query = $this->buildQuery();
      $this->queryArguments = $query->getArguments();
      $this->queryResult = $query->execute();
    }
    return $this->queryResult;
  }

  /**
   * Returns query arguments of the last executed query.
   *
   * SensorDatabaseAggregator::getQueryResult() must be called first.
   *
   * @return array
   *   The query arguments as an array.
   */
  protected function getQueryArguments() {
    return $this->queryArguments;
  }

  /**
   * {@inheritdoc}
   */
  public function resultVerbose(SensorResultInterface $result) {
    return "Query:\n{$this->getQueryResult()->getQueryString()}\n\nArguments:\n" . var_export($this->getQueryArguments(), TRUE);
  }

  /**
   * Get fetched object from the executed query.
   *
   * Multiple calls to executedQuery->fetchObjects() resulted in second call
   * returning false value.
   *
   * @return object
   */
  public function fetchObject() {
    if ($this->fetchedObject === NULL) {
      $this->fetchedObject = $this->getQueryResult()->fetchObject();
    }
    return $this->fetchedObject;
  }

  /**
   * Builds the database query.
   *
   * @return \SelectQuery
   */
  protected function buildQuery() {
    $table = $this->info->getSetting('table');
    $query = db_select($table);
    $this->buildQueryAggregate($query);

    if ($conditions = $this->info->getSetting('conditions')) {
      foreach ($conditions as $condition) {
        $query->condition($this->getFieldName($query, $condition), $condition['value'], isset($condition['operator']) ? $condition['operator'] : NULL);
      }
    }

    $time_interval_field = $this->info->getSetting('time_interval_field');
    if (!empty($time_interval_field) && $this->info->getTimeIntervalValue()) {
      $query->where(db_escape_field($time_interval_field) . ' > :timestamp_value',
        array(
          ':timestamp_value' => REQUEST_TIME - $this->info->getTimeIntervalValue(),
        ));
    }

    return $query;
  }

  /**
   * Returns the field name to use for a condition and ensures necessary joins.
   *
   * @param \SelectQueryInterface $query
   *   Select query instance.
   * @param array $condition
   *   A query condition array, containing at least the field.
   *
   * @return string
   *   The field name to use for conditions for that condition definition, can
   *   contain a table name alias if the field is part of a joined table.
   */
  protected function getFieldName(\SelectQueryInterface $query, array $condition) {
    if (strpos($condition['field'], '.') !== FALSE) {
      // Configurable field conditions are only supported if this is an entity
      // table.
      $entity_type = $this->getEntityTypeFromTable($this->info->getSetting('table'));
      list($field_name, $column) = explode('.', $condition['field']);

      // Add a join to the field table.
      $alias = $this->joinFieldTable($query, $entity_type, $field_name);

      // Return the combination of alias table name and field column.
      return $alias . '.' . _field_sql_storage_columnname($field_name, $column);
    }
    else {
      // A simple base table field.
      return $condition['field'];
    }
  }

  /**
   * Adds aggregate expressions to the query.
   *
   * Defaults to COUNT(*), override this method to use a different aggregation.
   *
   * @param \SelectQueryInterface $query
   *  The select query that should be aggregated.
   */
  protected function buildQueryAggregate(\SelectQueryInterface $query) {
    $query->addExpression('COUNT(*)', 'records_count');
  }

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    $query_result = $this->fetchObject();
    if (!empty($query_result)) {
      $records_count = $query_result->records_count;
    }
    else {
      $records_count = 0;
    }

    $result->setValue($records_count);
  }

  /**
   * Returns the entity type for a given base table.
   *
   * @param string $base_table
   *   The name of base table.
   *
   * @return string
   *   The entity type that is stored in the given base table.
   *
   * @throws \InvalidArgumentException
   *   Thrown if the table does not belong to any entity type.
   */
  protected function getEntityTypeFromTable($base_table) {
    foreach (entity_get_info() as $entity_type => $entity_info) {
      if (isset($entity_info['base table']) && $entity_info['base table'] == $base_table) {
        return $entity_type;
      }
    }

    // If we failed to find the entity type, abort.
    throw new \InvalidArgumentException(format_string('No entity type found for table @table', array('@table' => $this->info->getSetting('table'))));
  }

  /**
   * Joins the field data table for a given field.
   *
   * @param \SelectQueryInterface $query
   *   The select query which should be joined.
   * @param string $entity_type
   *   The entity type this field belongs to.
   * @param string $field_name
   *   Name of the field.
   *
   * @return string
   *   Alias of the joined table name.
   *
   * @throws \InvalidArgumentException
   *   Thrown if the given field does not use the SQL storage.
   */
  protected function joinFieldTable(\SelectQueryInterface $query, $entity_type, $field_name) {
    // Build the entity table and id column name.
    $entity_info = entity_get_info($entity_type);
    $entity_table_id = $this->info->getSetting('table') . '.' . $entity_info['entity keys']['id'];

    // Check the storage type.
    $field = field_info_field($field_name);
    if ($field['storage']['type'] != 'field_sql_storage') {
      throw new \InvalidArgumentException('Only configurable fields that use the sql storage are supported.');
    }

    // Add the join to the field data table.
    $table_name = _field_sql_storage_tablename($field);
    $alias = $query->join($table_name, $field_name, '%alias.entity_type = :entity_type AND %alias.entity_id = ' . $entity_table_id, array(':entity_type' => $entity_type));
    return $alias;
  }

  /**
   * Returns time interval options.
   *
   * @return array
   *   Array with time interval options, keyed by time interval in seconds.
   */
  protected function getTimeIntervalOptions() {
    return drupal_map_assoc(array(
      600,
      900,
      1800,
      3600,
      7200,
      10800,
      21600,
      32400,
      43200,
      64800,
      86400,
      172800,
      259200,
      604800,
      1209600,
      2419200
    ), 'format_interval') + array(0 => t('No restriction'));
  }

}
