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
 * Defines a wrapper for the mostpopular_services table.
 *
 * @author Andrew Marcus
 * @since Dec 18, 2009
 */
class MostPopularService {
  const STATUS_DISABLED = 0;
  const STATUS_CONFIGURED = 1;
  const STATUS_OK = 2;

  public static $table = 'mostpopular_services';
  public static $availableList;
  public static $enabledList;
  public static $disabledList;

  public $sid = 0;
  public $new = FALSE;
  public $enabled = 0;
  public $module = '';
  public $delta = '';
  public $name = '';
  public $title = '';
  public $extra = NULL;
  public $weight = 0;
  public $status = MostPopularService::STATUS_DISABLED;

  /**
   * Constructs a new service with the given values.
   *
   * @param array|object $object
   *   An object containing the values to include.
   */
  public function MostPopularService($object) {
    $this->update($object);
  }

  /**
   * Updates this service with the given values
   *
   * @param array|object $object
   *   An object containing the values to update.
   */
  public function update($object) {
    if (is_array($object)) {
      $object = (object)$object;
    }
    foreach ($object as $key => $val) {
      $this->$key = $val;
    }
    $this->new = ($this->sid == 0);
    $this->updateStatus($this->status, FALSE);
  }

  /**
   * Updates the status of this service.
   *
   * @param integer $status
   *   The new status.  Depending on whether the service is enabled, this value
   *   may be ignored.
   * @param boolean $save
   *   If this is true, the service will be saved if the status has changed.
   *   If false, you must save the service yourself.
   */
  public function updateStatus($status, $save = TRUE) {
    $old = $this->status;

    if (!$this->enabled) {
      $this->status = MostPopularService::STATUS_DISABLED;
    }
    elseif ($status == MostPopularService::STATUS_DISABLED) {
      $this->status = MostPopularService::STATUS_CONFIGURED;
    }
    else {
      $this->status = $status;
    }
    if ($save && $this->status != $old) {
      $this->save();
    }
  }

  /**
   * Saves the service, either updating an existing database record or creating
   * a new one.
   */
  public function save() {
    if ($this->sid > 0) {
      drupal_write_record(self::$table, $this, array('sid'));
    }
    else {
      drupal_write_record(self::$table, $this);

      // Get the new SID
      $this->sid = db_last_insert_id(self::$table, 'sid');
    }
  }

  /**
   * Fetches the service with the given service ID.
   *
   * @param integer $sid
   *   The service ID of the service to fetch.
   *
   * @return MostPopularService
   *   The service with the given service ID, or null if none could be found.
   */
  public static function fetch($sid) {
    $sid = (int)$sid;
    self::fetchAvailable();

    if (self::$enabledList[$sid]) {
      return self::$enabledList[$sid];
    }
    if (self::$disabledList[$sid]) {
      return self::$disabledList[$sid];
    }
    return NULL;
  }

  /**
   * Fetches the service with the given module and delta.
   *
   * @param string $module
   *   The module defining the service.
   * @param string $delta
   *   The delta of the service within the module.
   *
   * @return MostPopularService
   *   The service with the given service ID, or null if none could be found.
   */
  public static function fetchByModule($module, $delta) {
    self::fetchAvailable();

    if (isset(self::$availableList[$module][$delta])) {
      return self::$availableList[$module][$delta];
    }
    return NULL;
  }

  /**
   * Clears the internal caches, so the list of services must be fetched again.
   */
  public static function clear() {
    self::$availableList = NULL;
    self::$enabledList = NULL;
    self::$disabledList = NULL;
  }

  /**
   * Resets the titles for each of the services to the defaults.
   */
  public static function reset() {
    // Remove the titles from the database
    $sql = 'UPDATE {' . self::$table . '} SET title = null';
    db_query($sql);

    // Clear the cache and fetch the services again
    self::clear();
    $services = self::fetchSorted();
    foreach ($services as $service) {
      $service->save();
    }
  }

  /**
   * Fetches all of the available services from the enabled modules that define
   * them.
   */
  public static function fetchAvailable() {
    if (!isset(self::$availableList)) {
      self::$availableList = array();
      self::$enabledList = array();
      self::$disabledList = array();

      // Fetch all of the service definitions from the modules
      $modules = module_implements('mostpopular_service');
      foreach ($modules as $module) {
        $s = module_invoke($module, 'mostpopular_service', 'list');
        foreach ($s as $delta => $service) {
          $service['module'] = $module;
          $service['delta'] = $delta;

          // Augment the service with stored data from the database
          $sql = "SELECT * FROM {" . self::$table . "} ";
          $sql .= "WHERE module = '%s' AND delta = '%s'";
          $result = db_query($sql, $module, $delta);

          if ($row = db_fetch_array($result)) {
            foreach ($row as $key => $val) {
              if (isset($val) && drupal_strlen(trim($val)) > 0) {
                $service[$key] = $val;
              }
            }
          }
          $service = new MostPopularService($service);

          // If we've never seen the service before, add it first
          if ($service->new) {
            $service->save();
          }

          // Store the service by module and delta.
          self::$availableList[$module][$delta] = $service;

          // Store the service by availability
          if ($service->enabled) {
            self::$enabledList[$service->sid] = $service;
          }
          else {
            self::$disabledList[$service->sid] = $service;
          }
        }
      }
      // Sort the list of enabled services by weight
      uasort(self::$enabledList, '_mostpopular_service_sort');
    }
    return self::$availableList;
  }

  /**
   * Fetches all of the enabled services.
   */
  public static function fetchEnabled() {
    self::fetchAvailable();
    return self::$enabledList;
  }

  /**
   * Fetches all of the disabled services.
   */
  public static function fetchDisabled() {
    self::fetchAvailable();
    return self::$disabledList;
  }

  /**
   * Fetches all of the configured services.  The enabled services will appear
   * first, sorted by weight, followed by the disabled services sorted by
   * module.
   */
  public static function fetchSorted() {
    self::fetchAvailable();
    return array_merge(self::$enabledList, self::$disabledList);
  }

  /**
   * Returns the number of configured services, both enabled and disabled.
   */
  public static function numServices() {
    self::fetchAvailable();
    return count(self::$enabledList) + count(self::$disabledList);
  }

  /**
   * Returns the default service
   *
   * @return MostPopularService
   *   The enabled service with the lowest weight.
   */
  public static function getDefault() {
    self::fetchAvailable();
    foreach (self::$enabledList as $s) {
      return $s;
    }
    return NULL;
  }
}

/**
 * A custom sort function for the most popular services, that orders them
 * by weight.
 */
function _mostpopular_service_sort($s1, $s2) {
  $w1 = (int)$s1->weight;
  $w2 = (int)$s2->weight;
  return $w1 - $w2;
}