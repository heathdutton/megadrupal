<?php

/**
 * @file
 * Contains TetherStatsChartsSteppedChart.
 */

require_once __DIR__ . '/TetherStatsChartsInvalidDomainStepException.php';
require_once __DIR__ . '/TetherStatsChartsChart.php';
require_once __DIR__ . '/../schema/TetherStatsChartsSteppedChartSchema.php';

/**
 * Abstract class for a chart with domain steps.
 */
abstract class TetherStatsChartsSteppedChart extends TetherStatsChartsChart {

  /**
   * The DateTime marking when the data period ends.
   *
   * @var DateTime
   */
  protected $dateFinish;

  /**
   * Builds a TetherStatsChartsSteppedChart object based on the given schema.
   *
   * @param TetherStatsChartsSteppedChartSchema $schema
   *   The schema object which describes what kind of chart to build.
   * @param DateTime $date_start
   *   The date from which to start compiling data from the stats tables. The
   *   end date will be determined by the length of domain set out in the
   *   schema.
   * @param string|null $database_id
   *   The id of the database to use. Setting as '' or 'default' will use the
   *   default database. Null will use whatever is set as the current stats
   *   database by calling tether_stats_set_db().
   *
   * @throws TetherStatsChartsInvalidDomainStepException
   *   A proper domainStep must have been set.
   */
  public function __construct(TetherStatsChartsSteppedChartSchema $schema, DateTime $date_start, $database_id = NULL) {

    parent::__construct($schema, $date_start, $database_id);

    $step_types = array(
      'hour',
      'day',
      'month',
      'year',
    );

    if (!$this->schema->domainStep || !in_array($this->schema->domainStep, $step_types)) {

      throw new TetherStatsChartsInvalidDomainStepException();
    }
    else {

      TetherStatsChartsSteppedChartSchema::normalizeDate($this->schema->domainStep, $this->dateStart);
      $this->dateFinish = clone $this->dateStart;
      $this->schema->addStepSize($this->dateFinish, $this->schema->domainTicks);
    }
  }

}
