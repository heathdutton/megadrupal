<?php

/**
 * @file
 * Contains TetherStatsChartsComboChart.
 */

require_once __DIR__ . '/TetherStatsChartsSteppedChart.php';
require_once __DIR__ . '/../schema/TetherStatsChartsComboChartSchema.php';

/**
 * Class for a combo chart which uses both columns and line series.
 *
 * Compiles stats data and renders a chart as described by a
 * TetherStatsChartsComboChartSchema object.
 *
 * @see TetherStatsChartsComboChartSchema
 */
class TetherStatsChartsComboChart extends TetherStatsChartsSteppedChart {

  /**
   * Builds a new TetherStatsChartsComboChart object based on the given schema.
   *
   * Setting $database_id as "" or "default" will use the default database.
   *
   * @param TetherStatsChartsComboChartSchema $schema
   *   The schema object which describes what kind of chart to build.
   * @param DateTime $date_start
   *   The date from which to start compiling data from the stats tables.
   *   The end date will be determined by the length of domain set out in
   *   the schema.
   * @param string|null $database_id
   *   The id of the database to use. Setting as '' or 'default' will use the
   *   default database. Null will use whatever is set as the current stats
   *   database by calling tether_stats_set_db().
   */
  public function __construct(TetherStatsChartsComboChartSchema $schema, DateTime $date_start, $database_id = NULL) {

    parent::__construct($schema, $date_start, $database_id);
  }

  /**
   * Get a renderable array to build a stats chart using the charts module.
   *
   * @return array
   *   A renderable array.
   */
  public function getRenderable() {

    $transposed_data = $this->getTransposedDataMatrix();

    $chart = array(
      '#id' => $this->schema->chartId,
      '#type' => 'chart',
      '#chart_type' => 'column',
      '#data_labels' => TRUE,
      '#tooltips' => FALSE,
    );

    foreach ($transposed_data as $inx => $data_row) {

      $column_data = array();

      foreach ($data_row as $step => $value) {

        $column_data[] = $value;
      }

      $num_columns = count($this->schema->getChartableItemSpec());

      if ($inx < $num_columns) {

        $chart["column_{$inx}"] = array(
          '#type' => 'chart_data',
          '#title' => $this->schema->getChartableItemTitle($inx),
          '#data' => $column_data,
        );
      }
      else {

        $line_inx = $inx - $num_columns;
        $series = $this->schema->getSeriesSpec($line_inx);

        $chart["line_{$line_inx}"] = array(
          '#type' => 'chart_data',
          '#chart_type' => 'line',
          '#title' => $series['title'],
          '#data' => $column_data,
        );
      }
    }

    $chart['xaxis'] = array(
      '#type' => 'chart_xaxis',
      '#labels' => array(),
    );

    $steps = array_keys($this->dataMatrix);

    foreach ($steps as $step) {

      $step_time = new DateTime();
      $step_time->setTimestamp($step);

      $chart['xaxis']['#labels'][] = $this->getAxisStepLabel($step_time);
    }

    $chart['yaxis'] = array(
      '#type' => 'chart_yaxis',
      '#axis_type' => 'linear',
    );

    return $chart;
  }

  /**
   * Convert the raw dataMatrix into a chart data table.
   *
   * @return array
   *   An array containing:
   *   array(
   *     array($label, $valueColumnA, $valueColumnB...),
   *     ...
   *   )
   */
  public function getDataTable() {

    $this->loadData();

    $data_table = array();

    for ($start = clone $this->dateStart; $start < $this->dateFinish; $this->schema->addStepSize($start)) {

      if (!isset($this->dataMatrix[$start->getTimestamp()])) {

        $this->dataMatrix[$start->getTimestamp()] = array();
      }
      $row = $this->dataMatrix[$start->getTimestamp()];
      $label = $this->getAxisStepLabel($start);

      array_unshift($row, $label);
      $data_table[] = $row;
    }

    return $data_table;
  }

  /**
   * Gets a human readable label for a domain axis step.
   *
   * @param DateTime $step
   *   The domain step time.
   *
   * @return string
   *   The domain label at the $step position.
   */
  public function getAxisStepLabel(DateTime $step) {

    switch ($this->schema->domainStep) {

      case 'hour':
        $label = format_date($step->getTimestamp(), 'custom', 'jS H\h');
        break;

      case 'day':
        $label = format_date($step->getTimestamp(), 'custom', 'M j');
        break;

      case 'month':
        $label = format_date($step->getTimestamp(), 'custom', 'M Y');
        break;

      case 'year':
      default:
        $label = format_date($step->getTimestamp(), 'custom', 'Y');
        break;

    }
    return $label;
  }

  /**
   * Gets a transpose of the two-dimensional $dataMatrix array.
   *
   * @return array
   *   A transpose of the dataMatrix property.
   */
  protected function getTransposedDataMatrix() {

    $this->loadData();
    $transposed_data = array();

    $num_data_items = count($this->schema->getChartableItemSpec()) + count($this->schema->getSeriesSpec());

    for ($i = 0; $i < $num_data_items; $i++) {

      $transposed_data[$i] = array();

      foreach ($this->dataMatrix as $step => $row) {

        $transposed_data[$i][$step] = $this->dataMatrix[$step][$i];
      }
    }
    return $transposed_data;
  }

  /**
   * Populates the dataMatrix.
   *
   * Queries data based on the column and series types added.
   */
  protected function calcDataMatrix() {

    $num_columns = count($this->schema->getChartableItemSpec());

    foreach ($this->schema->getChartableItemSpec() as $inx => $column) {

      $this->calcColumnData($inx, $column);
    }

    foreach ($this->schema->getSeriesSpec() as $inx => $line_series) {

      $this->calcSeriesData($inx + $num_columns, $line_series);
    }
  }

  /**
   * Populates the dataMatrix with data for the given column.
   *
   * @param int $data_inx
   *   The index of the data matrix to where the column data applies.
   * @param array $column
   *   The array specification for the column from the Schema object.
   */
  protected function calcColumnData($data_inx, array $column) {

    $analytics_callback_arguments = $column['callback arguments'];

    $analytics_callback_arguments = array_merge($analytics_callback_arguments, array(
      $this->dateStart->getTimestamp(),
      $this->dateFinish->getTimestamp(),
      $this->schema->domainStep,
    ));

    $column_data = call_user_func_array(array($this, $column['analytics callback']), $analytics_callback_arguments);

    if ($column_data !== FALSE) {

      $start = clone $this->dateStart;
      $next = clone $start;
      $this->schema->addStepSize($next);

      $this->dataMatrix[$start->getTimestamp()][$data_inx] = 0;

      foreach ($column_data as $step => $count) {

        // Initialize the count and fill in zero values for step times when
        // there is no data.
        while ($step >= $next->getTimestamp()) {

          $this->schema->addStepSize($start);
          $this->schema->addStepSize($next);
          $this->dataMatrix[$start->getTimestamp()][$data_inx] = 0;
        }

        $this->dataMatrix[$start->getTimestamp()][$data_inx] += (int) $count;
      }
      $this->schema->addStepSize($start);

      while ($start < $this->dateFinish) {

        $this->dataMatrix[$start->getTimestamp()][$data_inx] = 0;
        $this->schema->addStepSize($start);
      }
    }
  }

  /**
   * Calculate and add line series data to the data matrix.
   *
   * @param int $data_inx
   *   The index in the data matrix where the line series data is to be
   *   inserted.
   * @param array $line_series
   *   The array of options describing the line series from the schema.
   */
  protected function calcSeriesData($data_inx, array $line_series) {

    switch ($line_series['series_type']) {

      case TetherStatsChartsComboChartSchemaInterface::SERIES_SUMMATION:
        $this->calcTotalData($data_inx);
        break;

      case TetherStatsChartsComboChartSchemaInterface::SERIES_MEAN:
        $this->calcMeanData($data_inx);
        break;

    }
  }

  /**
   * Calculates values in the dataMatrix for a summation line series.
   *
   * Sums the data count for each column in the dataMatrix. Each column
   * must already be calculated at this point.
   *
   * @param int $data_inx
   *   The index in the data matrix where the line series data is to be
   *   inserted.
   */
  protected function calcTotalData($data_inx) {

    $start = clone $this->dateStart;

    while ($start < $this->dateFinish) {

      $count = 0;

      for ($i = 0; $i < count($this->schema->getChartableItemSpec()); $i++) {

        $count += $this->dataMatrix[$start->getTimestamp()][$i];
      }

      $this->dataMatrix[$start->getTimestamp()][$data_inx] = $count;
      $this->schema->addStepSize($start);
    }
  }

  /**
   * Calculates values in the dataMatrix for a mean line series.
   *
   * Averages the data count for each column in the dataMatrix. Each
   * column must already be calculated at this point.
   *
   * @param int $data_inx
   *   The index in the data matrix where the line series data is to be
   *   inserted.
   */
  protected function calcMeanData($data_inx) {

    $start = clone $this->dateStart;

    while ($start < $this->dateFinish) {

      $count = 0;
      $num_columns = count($this->getChartableItemSpec());

      for ($i = 0; $i < $num_columns; $i++) {

        $count += $this->dataMatrix[$start->getTimestamp()][$i];
      }

      $this->dataMatrix[$start->getTimestamp()][$data_inx] = ($count / $num_columns);
      $this->schema->addStepSize($start);
    }
  }

}
