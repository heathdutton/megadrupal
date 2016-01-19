<?php
/**
 * @file
 * Class for database tasks.
 */

require_once DRUPAL_ROOT . '/includes/database/mysql/install.inc';

/**
 * Mysql Async implementation DatabaseTasks.
 */
class DatabaseTasks_mysql_async extends DatabaseTasks_mysql {
  /**
   * Returns a human-readable name string for MySQL and equivalent databases.
   */
  public function name() {
    return parent::name() . ' (' . st('Async support') . ')';
  }

}
