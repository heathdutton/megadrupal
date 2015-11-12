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
 * Defines a wrapper for the mostpopular_last_run table.
 *
 * @author Andrew Marcus
 * @since Jan 4, 2010
 */
class MostPopularLastRun {

  public static $table = 'mostpopular_last_run';
  public static $default_throttles = array();

  public $sid;
  public $iid;
  public $last_run = 0;
  public $throttle = NULL;
  public $new = FALSE;

  /**
   * Constructs a new MostPopularLastRun object, only used internally.
   *
   * @param array|object $object
   *   An object containing values to populate this object with.
   * @param boolean $new
   *   True if this is a new object, false if it already exists in the database.
   */
  protected function MostPopularLastRun($object, $new = FALSE) {
    if (is_array($object)) {
      $object = (object)$object;
    }
    $this->sid = $object->sid;
    $this->iid = $object->iid;
    $this->last_run = $object->last_run;
    $this->throttle = $object->throttle;
    $this->new = $new;

    // If the throttle isn't set, use the defaults instead
    if (!isset($this->throttle)) {
      $defaults = self::getDefaultThrottles($this->sid);
      $this->throttle = $defaults[$this->sid][$this->iid];
    }
  }

  /**
   * Can this service run again over this interval?
   *
   * @return boolean
   *   True if this service can run again for this interval, false otherwise.
   */
  public function canRun() {
    return empty($this->throttle) ||
      strtotime($this->throttle, $this->last_run) <= time();
  }

  /**
   * Returns the timestamp of the next time the service can run.
   *
   * If the service can run now (because no throttle was set or the throttle
   * interval has expired), returns the current timestamp.
   *
   * @return boolean
   *   The timestamp of the next time the service can run, which will always
   *   be greater than or equal to the current time.
   */
  public function nextRun() {
    if (empty($this->throttle)) {
      return time();
    }
    return max(strtotime($this->throttle, $this->last_run), time());
  }

  /**
   * Saves the last run info into the database.
   */
  public function save() {
    if ($this->new) {
      drupal_write_record(self::$table, $this);
    }
    else {
      drupal_write_record(self::$table, $this, array('sid', 'iid'));
    }
  }

  /**
   * Removes the last run and throttle info from the database.
   *
   * This should only be called if the service or interval has been removed.
   */
  public function remove() {
    $sql = 'DELETE FROM {' . self::$table . '} WHERE sid = %d AND iid = %d';
    db_query($sql, $this->sid, $this->iid);
  }

  /**
   * Creates new last run data for the given service and interval.
   *
   * The last run will be set to 0, and the default throttles will be used.
   * You must call save() on the resulting object before these values are
   * stored to the database.
   *
   * @param integer $sid
   *   The service ID.
   * @param integer $iid
   *   The interval ID.
   */
  public static function create($sid, $iid) {
    return new MostPopularLastRun(array(
      'sid' => $sid,
      'iid' => $iid,
      'last_run' => 0,
    ), TRUE);
  }

  /**
   * Clears the internal caches.
   */
  public static function clear() {
    self::$default_throttles = array();
  }

  /**
   * Invokes hook_mostpopular_service('throttles') on the given service to get
   * the default throttles to use for the currently-configured intervals.
   *
   * @param integer $sid
   *   The service ID.
   * @return array
   *   An array, hashed by $sid and by $iid, of the default throttle
   *   strtotime() strings.
   */
  public static function getDefaultThrottles($sid) {
    if (!isset(self::$default_throttles[$sid])) {
      $service = MostPopularService::fetch($sid);
      $intervals = MostPopularInterval::fetchAll();

      $options = array();
      foreach ($intervals as $interval) {
        $options[$interval->iid] = $interval->timestamp();
      }
      $out = module_invoke($service->module, 'mostpopular_service',
        'throttles', $service->delta, $options);

      foreach ($intervals as $interval) {
        if (!isset($out[$interval->iid])) {
          $out[$interval->iid] = '';
        }
      }
      self::$default_throttles[$sid] = $out;
    }
    return self::$default_throttles;
  }

  /**
   * Fetches the last run data for the given service or interval.
   *
   * @param integer $sid
   *   The service ID.  If null, all services are returned.
   * @param integer $iid
   *   The interval ID.  If null, all intervals are returned.
   *
   * @return array<MostPopularLastRun>
   *   An array of last run data for the given service and interval.
   */
  public static function fetchAll($sid = NULL, $iid = NULL) {
    $where = array();
    $params = array();
    if (isset($sid)) {
      $where[] = 'sid = %d';
      $params[] = (int)$sid;
    }
    if (isset($iid)) {
      $where[] = 'iid = %d';
      $params[] = (int)$iid;
    }
    $sql = 'SELECT * FROM {' . self::$table . '}';
    if (count($where) > 0) {
      $sql .= ' WHERE ' . implode(' AND ', $where);
    }

    $out = array();
    $result = db_query($sql, $params);
    while ($row = db_fetch_object($result)) {
      $out[] = new MostPopularLastRun($row);
    }
    return $out;
  }

  /**
   * Fetches the last run data for the given service and interval.
   *
   * If there is not yet any saved data for this service and interval,
   * this function creates and returns new data.
   *
   * @param integer $sid
   *   The service ID.
   * @param integer $iid
   *   The interval ID.
   *
   * @return MostPopularLastRun
   *   The data for the last time the service was run, or a new object if the
   *   service has not yet run.
   */
  public static function fetch($sid, $iid) {
    $out = array();
    $sql = 'SELECT * FROM {' . self::$table . '} WHERE sid = %d AND iid = %d';
    $result = db_query($sql, (int)$sid, (int)$iid);
    if ($row = db_fetch_object($result)) {
      return new MostPopularLastRun($row);
    }
    return self::create($sid, $iid);
  }

  /**
   * Resets the throttles for the given service to the defaults.
   *
   * @param integer $sid
   *   The service ID.  If null, all services are reset.
   * @param integer $iid
   *   The interval ID.  If null, all intervals are reset.
   */
  public static function resetThrottles($sid = NULL, $iid = NULL) {
    $runs = self::fetchAll($sid, $iid);

    foreach ($runs as $run) {
      $defaults = self::getDefaultThrottles($run->sid);

      if (isset($defaults[$run->sid][$run->iid])) {
        $run->throttle = $defaults[$run->sid][$run->iid];
        $run->save();
      }
      else {
        $run->remove();
      }
    }
  }

  /**
   * Resets the last time the specified service was run.
   *
   * @param integer $sid
   *   The service ID.  If null, all services are reset.
   * @param integer $iid
   *   The interval ID.  If null, all intervals are reset.
   */
  public static function resetLastRun($sid = NULL, $iid = NULL) {
    $runs = self::fetchAll($sid, $iid);

    foreach ($runs as $run) {
      $run->last_run = 0;
      $run->save();
    }
  }
}
