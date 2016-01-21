<?php

/**
 * @file
 * Contains TetherStatsChartsSteppedChartSchema.
 */

require_once __DIR__ . '/TetherStatsChartsInvalidDateIntervalSpecifiedException.php';
require_once __DIR__ . '/TetherStatsChartsSchema.php';
require_once __DIR__ . '/TetherStatsChartsIteratorInterface.php';

/**
 * Base schema class that describes a chart with domain steps.
 *
 * Extend this class to add chart specific schemas.
 */
class TetherStatsChartsSteppedChartSchema extends TetherStatsChartsSchema implements TetherStatsChartsIteratorInterface {

  /**
   * The number of ticks on the domain.
   *
   * The number of domain ticks or x-axis labels the chart should display data
   * for.
   *
   * @var int
   */
  public $domainTicks;

  /**
   * Size of each domain dtep.
   *
   * The base size of the domain step or the span of each x-axis label. Can be
   * one of 'hour', 'day', 'month' or 'year'.
   *
   * The size of each domain tick if $domainStepMultiplier multiplied by this
   * value.
   *
   * @var string
   */
  public $domainStep = FALSE;

  /**
   * Domain step multiplier.
   *
   * The size of each domain tick is $domainStep multiplied by this value.
   *
   * For example, a $domainStep of 'hour' with a $domainStepMultiplier of
   * 2 would produce a chart where each tick is 2 hours.
   *
   * @var int
   */
  public $domainStepMultiplier;

  /**
   * Constructs a new chart schema with the specified $chart_id.
   *
   * @param string $chart_id
   *   The unique id of the chart. Must be machine friendly. Used as a
   *   javascript variable.
   * @param int $domain_ticks
   *   The number of domain ticks or x-axis labels the chart should display
   *   data for.
   * @param string $domain_step
   *   The base size of the domain step or the span of each x-axis label. Can be
   *   one of 'hour', 'day', 'month' or 'year'.
   * @param int $domain_step_multiplier
   *   A multiplier for the domain step to increase the bucket size of each
   *   tick. For example, a $domainStep of 'hour' with a $domainStepMultiplier
   *   of 2 would produce a chart where each tick is 2 hours.
   */
  public function __construct($chart_id, $domain_ticks = 24, $domain_step = 'hour', $domain_step_multiplier = 1) {

    $this->domainTicks = $domain_ticks;
    $this->domainStep = $domain_step;
    $this->domainStepMultiplier = $domain_step_multiplier;

    parent::__construct($chart_id);
  }

  /**
   * Set the $domain_step and $domainTicks properties.
   *
   * Calculate and set the $domain_step and $domainTicks properties
   * automatically by adjusting the chart's step size to fit between the two
   * given dates.
   *
   * @param DateTime $date_start
   *   The start date of the data period.
   * @param DateTime $date_finish
   *   The end date of the data period.
   * @param int $max_domain_ticks
   *   The maximum number of ticks to appear in the domain before increasing to
   *   a larger step size.
   *
   * @throws TetherStatsChartsInvalidDateIntervalSpecifiedException
   *   The $date_start must be less than the $date_finish.
   */
  public function calcDomainStep(DateTime $date_start, DateTime $date_finish, $max_domain_ticks = 12, $domain_step_threshold = 32) {

    if ($date_start > $date_finish) {

      throw new TetherStatsChartsInvalidDateIntervalSpecifiedException();
    }

    $timestamp_diff = $date_finish->getTimestamp() - $date_start->getTimestamp();
    $steps = array('hour', 'day', 'month', 'year');

    foreach ($steps as $step_size) {

      switch ($step_size) {

        case 'hour':

          $this->domainTicks = floor($timestamp_diff / 3600);
          break;

        case 'day':
          TetherStatsChartsSteppedChartSchema::normalizeDate('day', $date_start);
          $date_diff = $date_finish->diff($date_start);
          $this->domainTicks = $date_diff->days;
          break;

        case 'month':
          TetherStatsChartsSteppedChartSchema::normalizeDate('month', $date_start);
          $date_diff = $date_finish->diff($date_start);
          $this->domainTicks = 12 * $date_diff->y + $date_diff->m;
          break;

        case 'year':
          TetherStatsChartsSteppedChartSchema::normalizeDate('year', $date_start);
          $date_diff = $date_finish->diff($date_start);
          $this->domainTicks = $date_diff->y;
          break;

      }

      $this->domainStep = $step_size;

      if ($this->domainTicks <= $max_domain_ticks) {

        // Make sure we have at least one domain tick.
        if ($this->domainTicks == 0) {

          $this->domainTicks = 1;
        }
        break;
      }
    }

    // Reset the domain step multiplier.
    $this->domainStepMultiplier = 1;

    while ($this->domainStepMultiplier * $max_domain_ticks < $this->domainTicks) {

      $this->domainStepMultiplier++;
    }

    // Reduce the domain ticks based on the new multiplier. The domainTicks
    // is the actual number of ticks on the horizontal axis that will appear
    // on the chart.
    $this->domainTicks = ceil($this->domainTicks / $this->domainStepMultiplier);
  }

  /**
   * Adds the domain step size to the given date.
   *
   * @param DateTime $date
   *   The date to increase.
   * @param int $ticks
   *   The number of steps the date should be increased by.
   */
  public function addStepSize(DateTime &$date, $ticks = 1) {

    // Adjust the base tick value by applying the multiplier.
    $ticks *= $this->domainStepMultiplier;

    TetherStatsChartsSteppedChartSchema::addInterval($this->domainStep, $date, $ticks);
  }

  /**
   * Subtracts the domain step size from the given date.
   *
   * @param DateTime $date
   *   The date to reduce.
   * @param int $ticks
   *   The number of steps the date should be reduced by.
   */
  public function subStepSize(DateTime &$date, $ticks = 1) {

    // Adjust the base tick value by applying the multiplier.
    $ticks *= $this->domainStepMultiplier;

    TetherStatsChartsSteppedChartSchema::subInterval($this->domainStep, $date, $ticks);
  }

  /**
   * Gets human readable label for the domainStep used.
   *
   * @return string
   *   The human readable domain step.
   */
  public function getDomainStepLabel() {

    return TetherStatsChartsSteppedChartSchema::mapDomainStepToLabel($this->domainStep);
  }

  /**
   * Calculates a time iterated once before $iterator_time.
   *
   * @param DateTime $iterator_time
   *   The current iteration start time.
   *
   * @return DateTime
   *   The previous iteration start time.
   */
  public function previousDateTime(DateTime $iterator_time) {

    $previous_time = clone $iterator_time;
    TetherStatsChartsSteppedChartSchema::subStepSize($previous_time, $this->domainTicks);

    return $previous_time;
  }

  /**
   * Calculates a time iterated once after $iterator_time.
   *
   * @param DateTime $iterator_time
   *   The current iteration start time.
   *
   * @return DateTime
   *   The next iteration start time.
   */
  public function nextDateTime(DateTime $iterator_time) {

    $next_time = clone $iterator_time;
    TetherStatsChartsSteppedChartSchema::addStepSize($next_time, $this->domainTicks);

    return $next_time;
  }

  /**
   * Normalizes the given date to the domain step size.
   *
   * Descrease the $date parameter such that the domain starts according to a
   * precise fit with the domain_step. The domain_step property must be
   * specified or no changes will occur.
   *
   * @param string $domain_step
   *   The domain step in which to normalize. Can be 'hour', 'day', 'month' or
   *   'year'.
   * @param DateTime $date
   *   The date to normalize.
   */
  public static function normalizeDate($domain_step, DateTime &$date) {

    switch ($domain_step) {

      case 'hour':
        $date = new DateTime($date->format('Y-m-d H:00:00'));
        break;

      case 'day':
        $date = new DateTime($date->format('Y-m-d 00:00:00'));
        break;

      case 'month':
        $date = new DateTime($date->format('Y-m-01 00:00:00'));
        break;

      case 'year':
        $date = new DateTime($date->format('Y-01-01 00:00:00'));
        break;

    }
  }

  /**
   * Adds the domain step size to the given date.
   *
   * @param string $domain_step
   *   The domain step size. Can be 'hour', 'day', 'month' or 'year'.
   * @param DateTime $date
   *   The date to modify.
   * @param int $ticks
   *   The number of times to add the domain_step size to the $date parameter.
   */
  public static function addInterval($domain_step, DateTime &$date, $ticks = 1) {

    // Add the given step size to the date depending on the step type.
    switch ($domain_step) {

      case 'hour':
        $date->add(new DateInterval("PT{$ticks}H"));
        break;

      case 'day':
        $date->add(new DateInterval("P{$ticks}D"));
        break;

      case 'month':
        $date->add(new DateInterval("P{$ticks}M"));
        break;

      case 'year':
        $date->add(new DateInterval("P{$ticks}Y"));
        break;

    }
  }

  /**
   * Subtracts the domain step size from the given date.
   *
   * @param string $domain_step
   *   The domain step size. Can be 'hour', 'day', 'month' or 'year'.
   * @param DateTime $date
   *   The date to modify.
   * @param int $ticks
   *   The number of times to subtract the domain_step size from the $date
   *   parameter.
   */
  public static function subInterval($domain_step, DateTime &$date, $ticks = 1) {

    // Subtract the given step size from the date depending on the step type.
    switch ($domain_step) {

      case 'hour':
        $date->sub(new DateInterval("PT{$ticks}H"));
        break;

      case 'day':
        $date->sub(new DateInterval("P{$ticks}D"));
        break;

      case 'month':
        $date->sub(new DateInterval("P{$ticks}M"));
        break;

      case 'year':
        $date->sub(new DateInterval("P{$ticks}Y"));
        break;

    }
  }

  /**
   * Maps a string id for a domain step to a human readable label.
   *
   * @param string $domain_step
   *   The domain step identifier.
   *
   * @return string
   *   The human readable domain step label.
   */
  public static function mapDomainStepToLabel($domain_step) {

    switch ($domain_step) {

      case 'hour':
        $ret_value = t('Hour');
        break;

      case 'day':
        $ret_value = t('Day');
        break;

      case 'month':
        $ret_value = t('Month');
        break;

      case 'year':
      default:
        $ret_value = t('Year');
        break;

    }
    return $ret_value;
  }

}
