<?php
/**
 * @file
 * Classes for insert and truncate.
 */

require_once DRUPAL_ROOT . '/includes/database/mysql/query.inc';

/**
 * Mysql Async implementation InsertQuery.
 */
class InsertQuery_mysql_async extends InsertQuery_mysql {}

/**
 * Mysql Async implementation TruncateQuery.
 */
class TruncateQuery_mysql_async extends TruncateQuery_mysql {}
