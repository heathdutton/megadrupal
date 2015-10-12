<?php

/**
 * @file
 * Contains TetherStatsChartsPieChart.
 */

require_once __DIR__ . '/TetherStatsChartsChart.php';
require_once __DIR__ . '/../schema/TetherStatsChartsPieChartSchema.php';

/**
 * Class for a pie chart.
 *
 * Compiles stats data and renders a chart as described by a
 * TetherStatsChartsPieChartSchema object.
 *
 * @see TetherStatsChartsPieChartSchema
 */
class TetherStatsChartsPieChart extends TetherStatsChartsChart {

  /**
   * The end of the data period to chart.
   *
   * @var DateTime
   */
  protected $dateFinish;

  /**
   * Builds a new TetherStatsChartsPieChart object based on the given schema.
   *
   * @param TetherStatsChartsPieChartSchema $schema
   *   The schema object which describes what kind of chart to build.
   * @param DateTime $date_start
   *   The date which starts the period of data from which to build the chart.
   * @param string|null $database_id
   *   The id of the database to use. Setting as '' or 'default' will use the
   *   default database. Null will use whatever is set as the current stats
   *   database by calling tether_stats_set_db().
   */
  public function __construct(TetherStatsChartsPieChartSchema $schema, DateTime $date_start, $database_id = NULL) {

    parent::__construct($schema, $date_start, $database_id);

    $this->dateFinish = clone $date_start;
    $this->dateFinish->add($schema->domainInterval);
  }

  /**
   * Get a renderable array to build a stats chart using the charts module.
   *
   * @return array
   *   A renderable array for the chart.
   */
  public function getRenderable() {

    $this->loadData();

    $chart = array(
      '#id' => $this->schema->chartId,
      '#type' => 'chart',
      '#chart_type' => 'pie',
      '#data_labels' => TRUE,
      '#tooltips' => FALSE,
    );
    $chart['pie_data'] = array(
      '#type' => 'chart_data',
      '#title' => $this->schema->sliceLabel,
    );

    foreach ($this->schema->getChartableItemSpec() as $inx => $column_info) {

      $chart['pie_data']['#labels'][] = $column_info['title'];
      $chart['pie_data']['#data'][] = $this->dataMatrix[$inx];
    }

    return $chart;
  }

  /**
   * Convert the raw dataMatrix into a chart data table.
   *
   * @return array
   *   An array containing:
   *     array(
   *       array($label, $value),
   *       ...
   *     )
   */
  public function getDataTable() {

    $this->loadData();

    $data_table = array();

    foreach ($this->dataMatrix as $inx => $value) {

      $data_table[] = array($this->schema->getChartableItemTitle($inx), $value);
    }

    return $data_table;
  }

  /**
   * Populates the dataMatrix.
   *
   * Queries data based on the column and series types added.
   */
  protected function calcDataMatrix() {

    foreach ($this->schema->getChartableItemSpec() as $inx => $column) {

      $this->calcSliceData($inx, $column);
    }
  }

  /**
   * Populates the dataMatrix with data for the given column.
   *
   * @param int $data_inx
   *   The index of the dataMatrix where to insert the column data.
   * @param array $slice
   *   An array specifying the slice from the Schema object.
   */
  protected function calcSliceData($data_inx, array $slice) {

    $analytics_callback_arguments = $slice['callback arguments'];

    $analytics_callback_arguments = array_merge($analytics_callback_arguments, array(
      $this->dateStart->getTimestamp(),
      $this->dateFinish->getTimestamp(),
    ));

    $count = call_user_func_array(array($this, $slice['analytics callback']), $analytics_callback_arguments);

    $this->dataMatrix[$data_inx] = (int) $count;
  }

}
