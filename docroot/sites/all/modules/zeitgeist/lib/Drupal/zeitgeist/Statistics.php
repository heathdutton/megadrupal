<?php

/**
 * @file
 * The model class for Zeitgeist statistics.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * Zeitgeist statistics.
 */
class Statistics {
  /**
   * @const 24 hours * 60 minutes * 60 seconds.
   */
  const ONEDAY = 86400;

  /**
   * @const The name of the table holding our data
   */
  const TABLE = 'zeitgeist';

  /**
   * @const Name of the variable holding the date of the latest history clear.
   */
  const VARLATESTCLEAR = 'zeitgeist_latest_clear';

  /**
   * Name of the variable holding the number of days the module keep its data.
   */
  const VARHISTORY = 'zeitgeist_history';

  /**
   * @var int
   *   A UNIX timestamp for the beginning of the time range.
   */
  public $ts1;
  /**
   * @var int
   *   A UNIX timestamp for the end of the time range.
   */
  public $ts2;
  /**
   * @var array
   *   An array of search scores during the time range.
   */
  public $scores = array();

  /**
   * Constructor.
   *
   * @param int $ts1
   *   Starting timestamp.
   * @param int $ts2
   *   Ending timestamp.
   */
  public function __construct($ts1, $ts2) {
    $this->ts1 = $ts1;
    $this->ts2 = $ts2;
  }

  /**
   * Add a score array to the statistics.
   *
   * @param string $search
   *   The actual search text.
   * @param string $category
   *   Search category.
   * @param int $count
   *   The number of occurrences of the search.
   */
  public function addScore($search, $category, $count) {
    $this->scores[] = array(
      'search'   => $search,
      'category' => $category,
      'count'    => $count,
    );
  }

  /**
   * Erases Zeitgeist log entries older than the chosen number of days.
   *
   * @see getLatestClearDate()
   *
   * @param int $days
   *   The number of days over which to clear Zeitgeist entries.
   * @param int $today
   *   The timestamp for "today 00:00". Optional.
   */
  public static function clearOlder($days, $today = NULL) {
    if (empty($today)) {
      $today = static::getDate();
    }
    $limit = static::getDate($today - $days * Statistics::ONEDAY);

    // This form is only called from cron, so no need for node access.
    db_delete(Statistics::TABLE)
      ->condition('ts', $limit, '<')
      ->execute();
    variable_set(Statistics::VARLATESTCLEAR, $today);
    watchdog('zeitgeist', 'Cleared ZG entries older than %days days',
      array('%days' => $days), WATCHDOG_NOTICE);
  }

  /**
   * Return the configured number of days for Zeitgeist history.
   *
   * @return int
   *   The number of days of Zeitgeist history to keep. 0 means forever.
   */
  public static function getHistoryDays() {
    return variable_get(Statistics::VARHISTORY, 0);
  }

  /**
   * Return links to the latest searches.
   *
   * Performance note: the unranged query used here draws attention because in
   * most cases developers expect it to be less efficient than a deduplicating
   * query using DISTINCT. However, on small servers typical of shared hosting,
   * and MySQL 3.x and 4.x, this query actually outperforms more complex ones in
   * spite of the extra data load involved.
   *
   * @todo Provide alternate queries depending on infra and a tool to check
   *   their respective performance levels.
   *
   * @see http://drupal.org/node/112681
   *
   * @param int $count
   *   The number of entries to return. 0 does not have any special meaning.
   *
   * @param bool $show_category
   *   Include the category information in search results.
   *
   * @return array
   *   search-key-indexed hash of searches.
   */
  public static function getLatestSearches($count, $show_category = FALSE) {
    $table = static::getTableName();
    $sql = <<<SQL
SELECT zg.search, zg.category
FROM {$table} zg
ORDER BY zg.ts DESC
SQL;

    // No access control on this table.
    $result = db_query($sql);
    $ret = array();

    // Since we use an unranged query, we weed out the duplicates on the fly,
    // and stop when we have enough results.
    foreach ($result as $row) {
      if ($count <= count($ret)) {
        break;
      }
      $category = $row->category;
      $search = $row->search;
      $key = !$show_category ? $search : t('@search (@category)', array(
        '@search' => $search,
        '@category' => $category,
      ));
      if (!empty($search) && !isset($ret[$key]) && Sanitization::validate($search, 'view')) {
        // Empty keys cannot happen: core search forms block them since D6.
        $ret[$key] = array($search, $category);
      }
    }

    // Because we use an unranged query, we must release the cursor ASAP.
    $result->closeCursor();

    return $ret;
  }

  /**
   * Returns a Zeitgeist statistics object.
   *
   * This is the most important function/feature of the module.
   *
   * @param int $span
   *   One of the predefined Drupal\zeitgeist\Span::* constants
   * @param int $timestamp
   *   A UNIX timestamp within the span. NULL means REQUEST_TIME.
   * @param string $category
   *   Restricts the type of searches taken into account (or not)
   * @param int $count
   *   Limits results to $count results, or does not limit if 0.
   *
   * @return Drupal\zeitgeist\Statistics
   *   The statistics over the chosen time range.
   */
  public static function getStatistics($span = Span::MONTH, $timestamp = NULL, $category = NULL, $count = 0) {
    list($ts1, $ts2) = Statistics::getSpanLimits($span, $timestamp);

    $table = static::getTableName();
    $params = array(
      ':tsmin' => $ts1,
      ':tsmax' => $ts2,
    );
    if (isset($category)) {
      $filter = '  AND (zg.category = :category)';
      $params[':category'] = $category;
    }
    else {
      $filter = '';
    }

    $sql = <<<SQL
SELECT zg.search, zg.category, count(zg.ts) cnt
FROM {$table} zg
WHERE (zg.ts >= :tsmin) AND (zg.ts < :tsmax)
$filter
GROUP BY 1, 2
ORDER BY 3 DESC, 1 ASC, 2 ASC
SQL;

    // No access control on this table.
    $result = ($count > 0) ? db_query_range($sql, 0, $count, $params) : db_query($sql, $params);

    $ret = new Statistics($ts1, $ts2);
    $ret->scores = array();
    foreach ($result as $o) {
      $ret->addScore($o->search, $o->category, $o->cnt);
    }
    return $ret;
  }

  /**
   * Return the latest Zeitgeist clear date.
   *
   * @return int
   *   The timestamp of the last time ::clearOlder() was invoked.
   */
  public static function getLatestClearDate() {
    return variable_get(Statistics::VARLATESTCLEAR, 0);
  }

  /**
   * Return the timestamp for the chosen date at 00:00.
   *
   * If !$ts, return today's date
   *
   * @see getSpanLimits()
   *
   * @param int $ts_now
   *   A timestamp within the day for which the 00:00 timestamp is requested.
   *   Zero means current time.
   *
   * @return int
   *   The timestamp for 00:00 on the same day as $ts_now.
   */
  public static function getDate($ts_now = 0) {
    $ar_day = $ts_now ? getdate($ts_now) : getdate();
    $ts_day = mktime(0, 0, 0, $ar_day['mon'], $ar_day['mday'], $ar_day['year']);
    return $ts_day;
  }

  /**
   * Return the timestamps bounding a given time span.
   *
   * @param Drupal\zeitgeist\Span $span
   *   The type of time span being considered.
   * @param int $timestamp
   *   A UNIX timestamp within the time span. NULL means REQUEST_TIME.
   *
   * @return array
   *   A [timestamp1, timestamps2] array.
   */
  public static function getSpanLimits($span = Span::MONTH, $timestamp = NULL) {
    if (!isset($timestamp)) {
      $timestamp = REQUEST_TIME;
    }
    $ar_date = getdate($timestamp);

    /* Some of these ranges go beyond the current timestamp if computed for the
     * current timestamp, but this is not an issue as there won't be any
     * matching entries anyway.
     */
    switch ($span) {
      case Span::DAY:
        $ts1 = Statistics::getDate($timestamp);
        $ts2 = $ts1 + Statistics::ONEDAY;
        break;

      case Span::WEEKS:
        $ts_base = $timestamp - $ar_date['wday'] * Statistics::ONEDAY;
        $ts1 = Statistics::getDate($ts_base);
        $ts2 = $ts1 + 7 * Statistics::ONEDAY;
        break;

      case Span::WEEKM:
        $ts_base = $timestamp - (($ar_date['wday'] + 6) % 7) * Statistics::ONEDAY;
        $ts1 = Statistics::getDate($ts_base);
        $ts2 = $ts1 + 7 * Statistics::ONEDAY;
        break;

      case Span::MONTH:
        $ts1 = mktime(0, 0, 0, $ar_date['mon'], 1, $ar_date['year']);
        // mktime() fixes out-of-range, so we needn't worry about year wrapping.
        $ts2 = mktime(0, 0, 0, $ar_date['mon'] + 1, 1, $ar_date['year']);
        break;

      case Span::QUARTER:
        $mon = 1 + (int) (($ar_date['mon'] - 1) / 3);
        $ts1 = mktime(0, 0, 0, $mon + 0, 1, $ar_date['year']);
        $ts2 = mktime(0, 0, 0, $mon + 3, 1, $ar_date['year']);
        break;

      case Span::YEAR:
        $ts1 = mktime(0, 0, 0, 1, 1, $ar_date['year']);
        $ts2 = mktime(0, 0, 0, 1, 1, $ar_date['year'] + 1);
        break;
    }

    return array($ts1, $ts2);
  }

  /**
   * Return the name of the model table.
   *
   * @return string
   *   The prefixed and escaped name of the table.
   */
  protected static function getTableName() {
    $ret = '{' . db_escape_table(static::TABLE) . '}';
    return $ret;
  }

  /**
   * Delegated implementation of the old settings hook.
   */
  public static function settings() {
    $ret = array(
      '#type'          => 'textfield',
      '#title'         => t('Number of days of history'),
      '#description'   => t('Historical data are kept for that number of days. 0 means forever. Pruning is performed by cron on a daily basis'),
      '#default_value' => static::getHistoryDays(),
      // This is good until which 2037, should be enough for all 7.x needs.
      '#size'          => 4,
      '#maxlength'     => 4,
    );

    return $ret;
  }

  /**
   * Stores a search.
   *
   * @param string $keys
   *   The search query.
   * @param string $kind
   *   The search type.
   * @param int $timestamp
   *   The search submission timestamp.
   */
  public static function store($keys, $kind, $timestamp = NULL) {
    // Make sure we do want to store an empty search.
    $do_store = empty($keys) ? Sanitization::isEmptySearchRecorded() : TRUE;

    if (!$do_store || !Sanitization::validate($keys, 'save')) {
      return;
    }

    if (!isset($timestamp)) {
      $timestamp = REQUEST_TIME;
    }

    $search = (object) array(
      'search'   => $keys,
      'category' => $kind,
      'ts'       => $timestamp,
    );

    drupal_write_record(static::TABLE, $search);
    // Since we added a new record, block contents may have changed.
    Cache::clearAll();
  }
}
