<?php
/**
 * @file
 * Cleaner base class file.
 */

/**
 * Class Cleaner.
 */
class Cleaner {
  /**
   * Static array with the time intervals.
   *
   * @var array
   */
  public static $intervals = array(
    900    => '15 min',
    1800   => '30 min',
    3600   => '1 hour',
    7200   => '2 hour',
    14400  => '4 hours',
    21600  => '6 hours',
    43200  => '12 hours',
    86400  => '1 day',
    172800 => '2 days',
    259200 => '3 days',
    604800 => '1 week',
  );

  /**
   * Helper function for gathering all names of cache tables in DB.
   *
   * @return array
   *   List of all cache tables names.
   *
   * @ingroup database
   */
  private static function cleanerGetCacheTables() {
    return db_query("SHOW TABLES LIKE 'cache%'")->fetchCol();
  }

  /**
   * Get the list of additional tables to clear.
   *
   * @return array
   *   Additional tables list.
   */
  private static function cleanerGetAdditionalTables() {
    $string = variable_get('cleaner_additional_tables', '');
    if (!empty($string)) {
      return explode(',', trim($string));
    }
  }

  /**
   * Helper function for getting all cache tables list in the table format.
   *
   * @return string
   *   Table HTML.
   *
   * @ingroup theme
   */
  public static function cleanerGetCacheTablesTable() {
    // Get all CACHE tables form database.
    $list = self::cleanerGetCacheTables();
    if (!empty($list)) {
      // Re-build cache tables list array by 4 items per array.
      $count = count($list);
      $cols = 4;
      $rows = ceil($count / $cols);
      $list = array_pad($list, $rows * $cols, ' ');
      $trows = array();
      for ($i = 0; $i < $count; $i += $cols) {
        $trows[] = array_slice($list, $i, $cols);
      }
      // Create theme table rendered HTML.
      $table = theme('table',
        array(
          'rows'       => $trows,
          'attributes' => array(
            'class' => array('cleaner-cache-tables'),
          ),
        )
      );
      return t('The current cache tables are:') . $table;
    }
    return t('There is no cache tables in the database.');
  }

  /**
   * Watchdog clearing handler.
   */
  public static function cleanerWatchdogClear() {
    if (db_query("TRUNCATE {watchdog}")->execute()) {
      self::cleanerLog('Cleared watchdog by Cleaner.');
    }
  }

  /**
   * Cache tables clearing handler.
   */
  public static function cleanerCacheClear() {
    $list = self::cleanerClearTables(self::cleanerGetCacheTables());
    if (!empty($list)) {
      // Write a log about successful cache clearing into the watchdog.
      self::cleanerLog('Cleared caches by Cleaner. (@list)',
        array('@list' => implode(', ', $list)));
    }
    else {
      // Write a log about empty list of cache tables into the watchdog.
      self::cleanerLog('There are no cache tables in the database.',
        array(), WATCHDOG_ERROR);
    }
  }

  /**
   * Clear tables handler.
   *
   * @param array $tables
   *   Table names array.
   *
   * @return array
   *   A list of successfully cleared tables.
   */
  private static function cleanerClearTables($tables) {
    $list = array();
    foreach ($tables as $table) {
      if (!db_table_exists($table)) {
        continue;
      }
      if (db_query("TRUNCATE $table")->execute()) {
        $list[] = $table;
      }
    }
    return $list;
  }

  /**
   * Clear additional tables.
   */
  public static function cleanerAdditionalTablesClear() {
    $list = self::cleanerClearTables(self::cleanerGetAdditionalTables());
    if (!empty($list)) {
      // Write a log about successful tables clearing into the watchdog.
      self::cleanerLog('Cleared tables by Cleaner. (@list)',
        array('@list' => implode(', ', $list)));
    }
    else {
      self::cleanerLog('There are no selected tables in the database.',
        array(), WATCHDOG_ERROR);
    }
  }

  /**
   * Sessions clearing handler.
   */
  public static function cleanerSessionsClear() {
    $cookie = session_get_cookie_params();
    $count = db_delete('sessions')
      ->condition('timestamp', REQUEST_TIME - $cookie['lifetime'], '<')
      ->execute();
    if ($count) {
      self::cleanerLog('Cleared @count sessions by Cleaner.',
        array('@count' => $count));
    }
  }

  /**
   * CSS files clearing handler.
   */
  public static function cleanerCssClear() {
    self::cleanerFilesClear('css');
  }

  /**
   * JS files clearing handler.
   */
  public static function cleanerJsClear() {
    self::cleanerFilesClear('js');
  }

  /**
   * MySQL tables optimizing handler.
   *
   * @param int $opt
   *   Operation flag.
   */
  public static function cleanerMysqlOptimizing($opt = 0) {
    $db_type = db_driver();
    // Make sure the db type hasn't changed.
    if ($db_type == 'mysql') {
      // Gathering tables list.
      $list = array();
      foreach (db_query("SHOW TABLE STATUS") as $table) {
        if ($table->Data_free) {
          $list[] = $table->Name;
        }
      }
      if (!empty($list)) {
        // Run optimization timer.
        timer_start('cleaner_db_optimization');
        // Execute optimize query.
        $query = 'OPTIMIZE ' . ($opt == 2 ? 'LOCAL ' : '');
        $query .= 'TABLE {' . (implode('}, {', $list)) . '}';
        db_query($query);
        // Write a log about successful optimization into the watchdog.
        self::cleanerLog('Optimized tables: !opts. This required !time seconds.',
          array(
            '!opts' => implode(', ', $list),
            '!time' => number_format((timer_read('cleaner_db_optimization') / 1000), 3),
          )
        );
      }
      else {
        // Write a log about thing that optimization process is
        // no tables which can to be optimized.
        self::cleanerLog('There is no tables which can to be optimized.',
          array(), WATCHDOG_NOTICE);
      }
    }
    else {
      // Write a log about thing that optimization process isn't allowed
      // for non-MySQL databases into the watchdog.
      self::cleanerLog('Database type (!type) not allowed to be optimized.',
        array('!type' => $db_type), WATCHDOG_ERROR);
    }
  }

  /**
   * Files cleaner handler.
   *
   * @param string $type
   *   Type: CSS or JS.
   */
  private static function cleanerFilesClear($type) {
    // Check if CSS/JS aggregation enabled.
    if (variable_get('preprocess_' . $type)) {
      $dir = drupal_realpath('public://' . $type);
      foreach (scandir($dir) as $file) {
        if (strpos($file, $type) !== FALSE) {
          $uri = 'public://' . $type . '/' . $file;
          if (REQUEST_TIME - filemtime($uri) > 3600) {
            file_unmanaged_delete($uri);
          }
        }
      }
      // Write a log about successful files clearing into the watchdog.
      self::cleanerLog('Cleared old temporary @type files by Cleaner',
        array('@type' => drupal_strtoupper($type)));
    }
    else {
      // Write a log about thing that CSS/JS aggregation isn't enabled.
      self::cleanerLog('Aggregation for @type files isn\'t enabled',
        array('@type' => drupal_strtoupper($type)));
    }
  }

  /**
   * Watchdog logging handler.
   *
   * @param string $message
   *   Message text.
   *
   * @param array $vars
   *   Variables array.
   *
   * @param int $severity
   *   Severity value.
   */
  private static function cleanerLog($message, $vars = array(), $severity = WATCHDOG_INFO) {
    watchdog('Cleaner', $message, $vars, $severity);
  }

}
