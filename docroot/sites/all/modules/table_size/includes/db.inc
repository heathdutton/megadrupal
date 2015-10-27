<?php
/**
 * @file
 * Provides a database utility class.
 */

/**
 * Retrieves and processes the database table information.
 */
class TableSizeDb {

  /**
   * The keys of the table status rows.
   *
   * These keys are also used to generate the table header in the admin page.
   */
  const COL_NAME    = 'name';
  const COL_COMMENT = 'comment';
  const COL_SIZE    = 'size (MB)';
  const COL_ROWS    = 'rows';

  /**
   * Retrieves and processes the database table information.
   *
   * @param bool $full
   *   If $full is TRUE, then all the SHOW TABLE STATUS information will be
   *   displayed. Typically, this will not be necessary.
   *
   * @return array
   */
  public static function showTableStatus($full = FALSE) {
    $results = db_query('SHOW TABLE STATUS');
    if ($full) {
      return $results->fetchAll(PDO::FETCH_ASSOC);
    }

    while ($row = $results->fetchAssoc()) {
      $rows[] = array(
        self::COL_NAME    => $row['name'],
        self::COL_COMMENT => $row['comment'] ? $row['comment'] : '&nbsp;',
        self::COL_SIZE    => ($row['data_length'] + $row['index_length']) / 1024 / 1024,
        self::COL_ROWS    => $row['rows'],
      );
    }
    return $rows;
  }

}