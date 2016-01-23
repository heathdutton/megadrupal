<?php

/**
 * @file
 * Provides generic functionality for the Tables Size module.
 */

/**
 * Provides a simple API for the table quotas.
 */
class TableSizeQuota {

  const STORAGE_KEY = 'table_size_quota';

  const TYPE_ROWS = 'rows';
  const TYPE_SIZE = 'size';

  private static $quota;
  private static $instance;

  private function __construct() {
    self::load();
  }
  private function __clone() {}
  private function __wakeup() {}

  /**
   * Returns an instance of TableSizeQuota.
   *
   * @return TableSizeQuota
   */
  public static function getInstance() {
    if (!self::$instance) {
      self::$instance = new TableSizeQuota();
    }
    return self::$instance;
  }

  /**
   * Returns the quota for the given table.
   *
   * @param string $table
   *   The name of the table.
   * @param string $type
   *   The type of quota to get. It can be TYPE_ROWS or TYPE_SIZE
   *
   * @return mixed
   *   The table quota or NULL if $table or $type are invalid.
   *
   * @see TableSizeQuota::getAll()
   * @see TableSizeQuota::getTable()
   */
  public function get($table, $type) {
    return isset(self::$quota[$table][$type]) ? self::$quota[$table][$type] : NULL;
  }

  /**
   * Returns the quota for the given table.
   *
   * @return array
   *
   * @see TableSizeQuota::get()
   * @see TableSizeQuota::getAll()
   */
  public function getTable($table) {
    return isset(self::$quota[$table]) ? self::$quota[$table] : NULL;
  }

  /**
   * Returns the quota for all the tables.
   *
   * @return array
   *
   * @see TableSizeQuota::get()
   * @see TableSizeQuota::getTable()
   */
  public function getAll() {
    return self::$quota;
  }

  /**
   * Loads the table quotas.
   */
  private function load() {
    self::$quota = LazyVars::get(self::STORAGE_KEY);
  }

  /**
   * Saves the quotas in the database.
   *
   * @param array $quotas
   */
  public function save(array $quotas) {
    self::$quota = $quotas;
    LazyVars::set(self::STORAGE_KEY, $quotas);
  }

}