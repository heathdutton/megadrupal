<?php
/**
 * @file
 * Time span enumeration class.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */


namespace Drupal\zeitgeist;

/**
 * Time spans for the results of Drupal\zeitgeist\Statistics::getStatistics().
 *
 * @see Drupal\zeitgeist\Statistics::getStatistics()
 */
class Span extends Enum {
  /**
   * @const Return zeitgeist for a day, starting at 00:00.
   */
  const DAY = 1;

  /**
   * @const Return zeitgeist for a calendar week, starting on Monday.
   */
  const WEEKM = 2;

  /**
   * @const Return zeitgeist for a calendar week, starting on Sunday.
   */
  const WEEKS = 3;

  /**
   * @const Return zeitgeist for a calendar month, starting on the 1st.
   */
  const MONTH = 4;

  /**
   * @const Return zeitgeist for a calendar quarter, starting on the 1st of Jan,
   * Apr, Jul, or Oct.
   */
  const QUARTER = 5;

  /**
   * @const Return zeitgeist for a calendar year, starting on the 1st.
   */
  const YEAR = 6;
}
