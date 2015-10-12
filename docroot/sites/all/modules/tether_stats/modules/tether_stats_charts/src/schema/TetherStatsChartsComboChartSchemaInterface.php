<?php

/**
 * @file
 * Contains TetherStatsChartsComboChartSchemaInterface.
 */

/**
 * Interface for TetherStatsChartsComboChartSchema.
 */
interface TetherStatsChartsComboChartSchemaInterface {

  /**
   * Summation line series type.
   *
   * This type will display a line series summing the counts of all columns in
   * this chart.
   *
   * @var int
   */
  const SERIES_SUMMATION = 0;

  /**
   * Mean line series type.
   *
   * This type will display a line series with the mean count of all columns
   * in this chart.
   *
   * @var int
   */
  const SERIES_MEAN = 1;

  /**
   * Gets the specification for a series in the chart.
   *
   * @param int|null $index
   *   The array index of the spec to retrieve.
   *
   * @return array
   *   The series spec. If $index is NULL, an array of all series specs will
   *   be returned.
   */
  public function getSeriesSpec($index = NULL);

  /**
   * Adds a summation line of the columns.
   *
   * @param string $title
   *   The title of the line series.
   *
   * @return $this
   */
  public function addSummationLineSeries($title);

  /**
   * Adds a mean line of the columns.
   *
   * @param string $title
   *   The title of the line series.
   *
   * @return $this
   */
  public function addMeanLineSeries($title);

}
