<?php

/**
 * @file
 * Contains TetherStatsChartsChart.
 */

require_once __DIR__ . '/TetherStatsChartsAnalyticsStorageInterface.php';
require_once __DIR__ . '/../schema/TetherStatsChartsSchema.php';

/**
 * Abstract class for all charts.
 */
abstract class TetherStatsChartsChart implements TetherStatsChartsAnalyticsStorageInterface {

  /**
   * The TetherStatsChartsSchema object used to define this chart.
   *
   * @var TetherStatsChartsSchema
   */
  public $schema;

  /**
   * The id of the database to use.
   *
   * @var string
   */
  protected $databaseId;

  /**
   * The chart's array of data points.
   *
   * @var array
   */
  protected $dataMatrix;

  /**
   * The start of the data period to chart.
   *
   * @var DateTime
   */
  protected $dateStart;

  /**
   * Builds a new TetherStatsChartsChart object based on the given schema.
   *
   * @param TetherStatsChartsSchema $schema
   *   The schema object which describes what kind of chart to build.
   * @param DateTime $date_start
   *   The start time for the data period.
   * @param string|null $database_id
   *   The id of the database to use. Setting as '' or 'default' will use the
   *   default database. Null will use whatever is set as the current stats
   *   database by calling tether_stats_set_db().
   */
  public function __construct(TetherStatsChartsSchema $schema, DateTime $date_start, $database_id = NULL) {

    if (!isset($database_id)) {

      $database_id = tether_stats_set_db();
    }

    $this->schema = $schema;
    $this->databaseId = $database_id;
    $this->dataMatrix = FALSE;
    $this->dateStart = clone $date_start;
  }

  /**
   * Queries the database and populates the dataMatrix.
   *
   * @param bool $reset
   *   Force the data matrix to reload even if the dataMatrix has already
   *   been populated.
   *
   * @return array
   *   An array containing the raw data matrix. This differs depending
   *   on the type of chart.
   */
  public function loadData($reset = FALSE) {

    if ($this->dataMatrix === FALSE || $reset) {

      $this->dataMatrix = array();
      $this->calcDataMatrix();
    }
    return $this->dataMatrix;
  }

  /**
   * Accessor for the start date.
   */
  public function getDateStart() {

    return clone $this->dateStart;
  }

  /**
   * Get a renderable array to build a stats chart using the charts module.
   *
   * @return array
   *   A renderable array.
   */
  abstract public function getRenderable();

  /**
   * Convert the raw dataMatrix into a chart data table.
   *
   * @return array
   *   A data table format used by the Charts module.
   */
  abstract public function getDataTable();

  /**
   * Populates the dataMatrix with data based on the schema.
   */
  abstract protected function calcDataMatrix();

  /**
   * {@inheritdoc}
   */
  public function getAllActivityCount($activity_type, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $select = db_select('tether_stats_hour_count', 'c')
      ->condition('type', $activity_type)
      ->condition('hour', $start, '>=')
      ->condition('hour', $finish, '<');

    $select->addExpression('SUM(count)', 'count');

    $ret_value = $this->executeDataQuery($select, $step);
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getElementActivityCount($elid, $activity_type, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $select = db_select('tether_stats_hour_count', 'c')
      ->condition('elid', $elid)
      ->condition('type', $activity_type)
      ->condition('hour', $start, '>=')
      ->condition('hour', $finish, '<');

    $select->addExpression('SUM(count)', 'count');

    $ret_value = $this->executeDataQuery($select, $step);
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getElementImpressedOnElementCount($elid_impressed, $elid_impressed_on, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $select = db_select('tether_stats_impression_log', 'i');
    $select->join('tether_stats_activity_log', 'a', 'i.alid = a.alid');

    $select->condition('i.elid', $elid_impressed)
      ->condition('a.elid', $elid_impressed_on)
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getElementImpressedOnNodeBundleCount($elid_impressed, $bundle, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $select = db_select('tether_stats_impression_log', 'i');
    $select->join('tether_stats_activity_log', 'a', 'i.alid = a.alid');
    $select->join('tether_stats_element', 'e', "a.elid = e.elid AND e.entity_type = 'node'");
    $select->join('node', 'n', 'e.entity_id = n.nid');

    $select->condition('i.elid', $elid_impressed)
      ->condition('n.type', $bundle, '=')
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getElementImpressedOnBaseUrlCount($elid_impressed, $base_url, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $select = db_select('tether_stats_impression_log', 'i');
    $select->join('tether_stats_activity_log', 'a', 'i.alid = a.alid');
    $select->join('tether_stats_element', 'e', "a.elid = e.elid");

    $select->condition('i.elid', $elid_impressed)
      ->condition('e.url', db_like($base_url) . '%', 'LIKE')
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getElementImpressedAnywhereCount($elid_impressed, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $select = db_select('tether_stats_hour_count', 'c')
      ->condition('elid', $elid_impressed)
      ->condition('type', 'impression')
      ->condition('hour', $start, '>=')
      ->condition('hour', $finish, '<');

    $select->addExpression('SUM(count)', 'count');

    $ret_value = $this->executeDataQuery($select, $step);
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getAllElementsImpressedOnElementCount($elid_impressed_on, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $select = db_select('tether_stats_impression_log', 'i');
    $select->join('tether_stats_activity_log', 'a', 'i.alid = a.alid');

    $select->condition('a.elid', $elid_impressed_on)
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getHitActivityWithReferrerCount($referrer, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $referrer = db_like($referrer);

    $select = db_select('tether_stats_activity_log', 'a')
      ->condition('a.type', 'hit')
      ->condition('a.referrer', "%{$referrer}%", 'LIKE')
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getElementHitActivityWithReferrerCount($elid, $referrer, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $referrer = db_like($referrer);

    $select = db_select('tether_stats_activity_log', 'a')
      ->condition('a.elid', $elid)
      ->condition('a.type', 'hit')
      ->condition('a.referrer', "%{$referrer}%", 'LIKE')
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getHitActivityWithBrowserCount($browser, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $browser = db_like($browser);

    $select = db_select('tether_stats_activity_log', 'a')
      ->condition('a.type', 'hit')
      ->condition('a.browser', "%{$browser}%", 'LIKE')
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * {@inheritdoc}
   */
  public function getElementHitActivityWithBrowserCount($elid, $browser, $start, $finish, $step = NULL) {

    $old_db = db_set_active($this->databaseId);
    $browser = db_like($browser);

    $select = db_select('tether_stats_activity_log', 'a')
      ->condition('a.elid', $elid)
      ->condition('a.type', 'hit')
      ->condition('a.browser', "%{$browser}%", 'LIKE')
      ->condition('a.created', $start, '>=')
      ->condition('a.created', $finish, '<');

    $select->addExpression('COUNT(*)', 'count');

    $ret_value = $this->executeDataQuery($select, $step, 'a');
    db_set_active($old_db);

    return $ret_value;
  }

  /**
   * Completes an executes an analytics data query.
   *
   * This is a helper method that will complete data select queries, which all
   * have common parts, then execute the query.
   *
   * @param SelectQuery $select
   *   The base select query to complete and execute.
   * @param string $step
   *   The domain step to aggregate over as defined in TetherStatsAnalytics.php.
   * @param string $table_alias
   *   The table alias where the step field comes from.
   *
   * @return int|array
   *   Returns the total count if the $step is NULL, otherwise returns an
   *   array of the unixtime of the domain step to the count for that step.
   */
  private function executeDataQuery(SelectQuery $select, $step, $table_alias = 'c') {

    if (isset($step)) {

      $select->addField($table_alias, $step, 'step');
      $select->groupBy('step');
      $select->orderBy('step');

      $result = $select->execute()->fetchAllKeyed();
    }
    else {

      $result = $select->execute()->fetchField();

      if (!isset($result)) {

        $result = 0;
      }
    }
    return $result;
  }

}
