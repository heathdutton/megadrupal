<?php

/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright © 2010 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com –or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */
/**
 * @file
 * Defines a wrapper for the mostpopular_intervals table.
 *
 * @author Andrew Marcus
 * @since Dec 18, 2009
 */
class MostPopularInterval {

  public static $table = 'mostpopular_intervals';

  public $iid = 0;
  public $title = '';
  public $string = '';
  public $weight = 0;

  /**
   * Constructs a new interval.
   *
   * If a row object is passed as the parameter, the $iid, $title, $interval
   * and $weight are all extracted from the object.
   * Otherwise, the parameters are used for these values.
   *
   * @param array|object $object
   *   An object with settings for this interval.
   */
  public function MostPopularInterval($object) {
    $this->update($object);
  }

  /**
   * Updates this service with the given values
   *
   * @param array|object $object
   *   An object containing the values to update.
   */
  public function update($object) {
    foreach ($object as $key => $val) {
      if (isset($this->$key)) {
        $this->$key = $val;
      }
    }
  }

  /**
   * Creates a new, empty interval with the given weight.
   *
   * @param integer $weight
   *   The weight to assign this interval.
   *
   * @return MostPopularInterval
   */
  public static function create($weight) {
    return new MostPopularInterval(array(
      'weight' => $weight,
    ));
  }

  /**
   * Returns the timestamp, relative to the current time, that marks
   * the start of this interval.
   *
   * @return integer
   *   A timestamp in the past, or 0 if there is no interval string specified.
   */
  public function timestamp() {
    return !empty($this->string) ? strtotime($this->string) : 0;
  }

  /**
   * Returns the full title, which is the interval's title prepended with
   * 'Past '. So, for instance, 'Day' becomes 'Past Day'.
   *
   * @return string
   *   The full title of the interval.
   */
  public function fullTitle() {
    return 'Past ' . $this->title;
  }

  /**
   * Stores this interval into the database.
   */
  public function save() {
    if ($this->iid > 0) {
      drupal_write_record(self::$table, $this, array( 'iid' ));
    }
    else {
      drupal_write_record(self::$table, $this);
      $this->iid = db_last_insert_id(self::$table, 'iid');
    }
  }

  /**
   * Removes this interval from the database.
   */
  public function remove() {
    if ($this->iid > 0) {
      $sql = 'DELETE FROM {' . self::$table . '} WHERE iid = %d';
      db_query($sql, $this->iid);
    }
  }

  /**
   * Removes all intervals from the database.
   */
  public static function clear() {
    db_query('TRUNCATE {' . self::$table . '}');
  }

  /**
   * Resets the list of intervals to the default settings.
   */
  public static function reset() {
    self::clear();

    $day = new MostPopularInterval(array(
      'title' => t('Day'),
      'string' => '-1 day',
      'weight' => 0));
    $day->save();

    $week = new MostPopularInterval(array(
      'title' => t('Week'),
      'string' => '-1 week',
      'weight' => 1));
    $week->save();

    $month = new MostPopularInterval(array(
      'title' => t('Month'),
      'string' => '-1 month',
      'weight' => 2));
    $month->save();

    $year = new MostPopularInterval(array(
      'title' => t('Year'),
      'string' => '-1 year',
      'weight' => 3));
    $year->save();
  }

  /**
   * Fetches all of the intervals from the database, ordered by weight.
   *
   * @return array<MostPopularInterval>
   *   A list of intervals, sorted by weight.
   */
  public static function fetchAll() {
    $intervals = array();

    $sql = 'SELECT * FROM {' . self::$table . '} ORDER BY weight ASC';
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
      $intervals[] = new MostPopularInterval($row);
    }
    return $intervals;
  }

  /**
   * Fetches an interval object for the given interval ID.
   *
   * @param integer $iid
   *   The interval ID.
   *
   * @return MostPopularInterval
   *   The interval with the given ID, or null if none could be found.
   */
  public static function fetch($iid) {
    $sql = 'SELECT * FROM {' . self::$table . '} WHERE iid = %d';
    $result = db_query($sql, (int)$iid);
    if ($row = db_fetch_object($result)) {
      return new MostPopularInterval($row);
    }
    return NULL;
  }

 /**
   * Returns the default interval.
   *
   * @return MostPopularInterval
   *   The interval with the lowest weight.
   */
  public static function getDefault() {
    $sql = 'SELECT * FROM {' . self::$table . '} ORDER BY weight ASC';
    $result = db_query_range($sql, 0, 1);
    if ($row = db_fetch_object($result)) {
      return new MostPopularInterval($row);
    }
    return NULL;
  }
}