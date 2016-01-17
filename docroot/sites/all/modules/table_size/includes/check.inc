<?php
/**
 * @file
 * Table Size checks / algorithms.
 */

/**
 * Checks table status vs table quotas.
 */
class TableSizeCheck {

  const KEY_QUOTA = 'quota';
  const KEY_ACTUAL = 'actual';
  const KEY_EXCEEDED = 'exceeded';
  const KEY_EXCEEDED_PERCENT = 'exceeded_percent';

  /**
   * Gets the tables that exceeded the quota.
   */
  public static function getTablesExceededQuota() {
    $quota = TableSizeQuota::getInstance();
    $q = $quota->getAll();
    $tables = TableSizeDb::showTableStatus();

    $exceeded_tables = array();
    $config = array(
      TableSizeDb::COL_SIZE => TableSizeQuota::TYPE_SIZE,
      TableSizeDb::COL_ROWS => TableSizeQuota::TYPE_ROWS,
    );
    foreach ($tables as $table) {
      $table_name = $table[TableSizeDb::COL_NAME];
      foreach ($config as $dbk => $qk) {
        if (isset($q[$table_name][$qk]) && $q[$table_name][$qk] && $table[$dbk] > $q[$table_name][$qk]) {
          $exceeded_tables[$table_name][$qk] = array(
            self::KEY_QUOTA => $q[$table_name][$qk],
            self::KEY_ACTUAL => $table[$dbk],
            self::KEY_EXCEEDED => $table[$dbk] - $q[$table_name][$qk],
            self::KEY_EXCEEDED_PERCENT => round(($table[$dbk] - $q[$table_name][$qk]) * 100 / $q[$table_name][$qk], 2),
          );
        }
      }
    }

    return $exceeded_tables;
  }

}