<?php
/**
 * @file
 * Class for select queries.
 */

/**
 * Mysql Async implementation SelectQuery.
 */
class SelectQuery_mysql_async extends SelectQuery {
  /**
   * Constructor.
   */
  public function __construct($table, $alias = NULL, DatabaseConnection $connection = NULL, $options = array()) {
    $connection_options = $connection->getConnectionOptions();
    $options += $connection_options['default_options_select'];
    parent::__construct($table, $alias, $connection, $options);
  }

}
