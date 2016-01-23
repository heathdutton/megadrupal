<?php
/**
 * @file
 * Class for database schema operations.
 */

require_once DRUPAL_ROOT . '/includes/database/mysql/schema.inc';

/**
 * Mysql Async implementation DatabaseSchema.
 */
class DatabaseSchema_mysql_async extends DatabaseSchema_mysql {
  /**
   * Check if table exists.
   *
   * @param string $table
   *   The table to check.
   *
   * @return bool
   *   TRUE if table exists, FALSE if not.
   */
  public function tableExists($table) {
    // The information_schema table is very slow to query under MySQL 5.0.
    // Instead, we try to select from the table in question.  If it fails,
    // the most likely reason is that it does not exist. That is dramatically
    // faster than using information_schema.
    // @link http://bugs.mysql.com/bug.php?id=19588
    // @todo: This override should be removed once we require a version of MySQL
    // that has that bug fixed.
    try {
      $this->connection->queryRange("SELECT 1 FROM {" . $table . "}", 0, 1)->fetchField();
      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Check if field exists.
   *
   * @param string $table
   *   The table to check.
   * @param string $column
   *   The field to check.
   *
   * @return bool
   *   TRUE if field exists in table, FALSE if not.
   */
  public function fieldExists($table, $column) {
    // The information_schema table is very slow to query under MySQL 5.0.
    // Instead, we try to select from the table and field in question. If it
    // fails, the most likely reason is that it does not exist. That is
    // dramatically faster than using information_schema.
    // @link http://bugs.mysql.com/bug.php?id=19588
    // @todo: This override should be removed once we require a version of MySQL
    // that has that bug fixed.
    try {
      $this->connection->queryRange("SELECT $column FROM {" . $table . "}", 0, 1)->fetchField();
      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

}
