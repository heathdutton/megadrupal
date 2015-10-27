<?php
/**
 * @file
 * Interface for schemas that support iterative charts.
 */

/**
 * Interface to support chart iteration.
 *
 * Chart schema classes may implement this interface to provide iteration on
 * charts using the schema.
 */
interface TetherStatsChartsIteratorInterface {
  /**
   * Calculates a time iterated once before $iterator_time.
   *
   * @param DateTime $iterator_time
   *   The start time of the current iteration position.
   *
   * @return DateTime
   *   The start time of the next iteration position.
   */
  public function previousDateTime(DateTime $iterator_time);

  /**
   * Calculates a time iterated once after $iterator_time.
   *
   * @param DateTime $iterator_time
   *   The start time of the current iteration position.
   *
   * @return DateTime
   *   The start time of the previous iteration position.
   */
  public function nextDateTime(DateTime $iterator_time);

}
