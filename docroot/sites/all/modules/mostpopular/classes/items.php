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
 * Defines a wrapper for the mostpopular_items table.
 *
 * @author Andrew Marcus
 * @since Jan 5, 2010
 */
class MostPopularItem {

  public static $table = 'mostpopular_items';

  public $sid;
  public $iid;
  public $nid;
  public $url;
  public $title;
  public $count;
  public $new = FALSE;

  /**
   * Constructs a new MostPopularItem with the given values.
   *
   * @param array|object $object
   *   An object containing values to populate this object with.
   * @param boolean $new
   *   True if this is a new object, false if it already exists in the database.
   */
  public function MostPopularItem($object, $new = FALSE) {
    if (is_array($object)) {
      $object = (object)$object;
    }
    $this->sid = $object->sid;
    $this->iid = $object->iid;
    $this->nid = $object->nid;
    $this->url = $object->url;
    $this->title = $object->title;
    $this->count = $object->count;
    $this->new = $new;
  }

  /**
   * Save the last run info, either updating an existing database record or
   * creating a new one.
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
   * Fetches the cached most popular items for the given service or interval.
   *
   * @param integer $sid
   *   The service ID.  If null, all services are used.
   * @param integer $iid
   *   The interval ID.  If null, all intervals are used.
   *
   * @return array<MostPopularItem>
   *   An array of the most popular items, sorted by the number of times they
   *   appeared.
   */
  public static function fetch($sid = NULL, $iid = NULL) {
    $where = array();
    $params = array();
    if (!empty($sid)) {
      $where[] = 'sid = %d';
      $params[] = $sid;
    }
    if (!empty($iid)) {
      $where[] = 'iid = %d';
      $params[] = $iid;
    }
    $sql = 'SELECT * FROM {' . self::$table . '}';
    if (count($where)) {
      $sql .= ' WHERE ' . implode(' AND ', $where);
    }
    $sql .= ' ORDER BY count DESC';
    $max = variable_get('mostpopular_max', 5);

    $result = db_query_range($sql, $params, 0, $max);
    $out = array();
    while ($row = db_fetch_object($result)) {
      $out[] = new MostPopularItem($row);
    }
    return $out;
  }

  /**
   * Clears the cached items for the given service and interval.
   *
   * @param integer $sid
   *   The service ID.  If null, all services are reset.
   * @param integer $iid
   *   The interval ID.  If null, all intervals are reset.
   */
  public static function reset($sid = NULL, $iid = NULL) {
    $where = array();
    $params = array();
    if (!empty($sid)) {
      $where[] = 'sid = %d';
      $params[] = $sid;
    }
    if (!empty($iid)) {
      $where[] = 'iid = %d';
      $params[] = $iid;
    }
    $sql = 'DELETE FROM {' . self::$table . '}';
    if (count($where)) {
      $sql .= ' WHERE ' . implode(' AND ', $where);
    }
    db_query($sql, $params);
  }
}
