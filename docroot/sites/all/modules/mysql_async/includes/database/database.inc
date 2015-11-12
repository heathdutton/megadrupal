<?php

/**
 * @file
 * Database interface code for MySQL asynchronous database servers.
 */

/**
 * @addtogroup database
 * @{
 */

require_once DRUPAL_ROOT . '/includes/database/mysql/database.inc';

require_once __DIR__ . '/async-exception.inc';
require_once __DIR__ . '/async-connection.inc';
require_once __DIR__ . '/async-iterator.inc';
require_once __DIR__ . '/async-log.inc';
require_once __DIR__ . '/async-result.inc';
require_once __DIR__ . '/async-statement.inc';

/**
 * Database connection class for MySQL.
 *
 * Based on the core MySQL implementation, this class adds support
 * for asynchronous queries, by accepting an 'async' parameter in
 * the options array for queries.
 */
class DatabaseConnection_mysql_async extends DatabaseConnection_mysql {
  public $currentResultId = NULL;

  /**
   * Constructor.
   *
   * Initialize known options with sane defaults.
   */
  public function __construct(array $connection_options = array()) {
    $connection_options += array(
      'async_max_connections' => 10,
      'async_min_connections' => 3,
      'default_options' => array(),
      'default_options_select' => array(),
      'pre_soft_reap' => TRUE,
    );

    $connection_options['default_options'] += array(
      'async' => FALSE,
      'async_transaction' => FALSE,
      'async_timeout' => 60,
      'async_require' => FALSE,
    );

    parent::__construct($connection_options);

    $this->connectionOptions += array(
      'mysqli' => array(),
    );

    $this->connectionOptions['mysqli'] += array(
      'host' => isset($connection_options['host']) ? $connection_options['host'] : NULL,
      'username' => isset($connection_options['username']) ? $connection_options['username'] : NULL,
      'password' => isset($connection_options['password']) ? $connection_options['password'] : NULL,
      'database' => isset($connection_options['database']) ? $connection_options['database'] : NULL,
      'port' => isset($connection_options['port']) ? $connection_options['port'] : NULL,
      'socket' => isset($connection_options['socket']) ? $connection_options['socket'] : NULL,
      'pdo' => isset($connection_options['pdo']) ? $connection_options['pdo'] : array(),
      'init_commands' => isset($connection_options['init_commands']) ? $connection_options['init_commands'] : array(),
      'collation' => isset($connection_options['collation']) ? $connection_options['collation'] : NULL,
    );

    // If socket is specified, tell mysqli_connect to use the socket,
    // by setting the host to NULL.
    if ($this->connectionOptions['mysqli']['socket']) {
      $this->connectionOptions['mysqli']['host'] = NULL;
    }

    // Set MySQL init_commands if not already defined.  Default Drupal's MySQL
    // behavior to conform more closely to SQL standards.  This allows Drupal
    // to run almost seamlessly on many different kinds of database systems.
    // These settings force MySQL to behave the same as postgresql, or sqlite
    // in regards to syntax interpretation and invalid data handling.  See
    // http://drupal.org/node/344575 for further discussion. Also, as MySQL 5.5
    // changed the meaning of TRADITIONAL we need to spell out the modes one by
    // one.
    $this->connectionOptions['mysqli']['init_commands'] += array(
      'sql_mode' => "SET sql_mode = 'ANSI,STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER'",
    );
  }

  /**
   * Wrapper for parent query.
   */
  public function parentQuery($query, $args, $options) {
    $this->currentResultId = NULL;
    return parent::query($query, $args, $options);
  }

  /**
   * Return the driver.
   *
   * @see DatabaseConnection::driver()
   */
  public function driver() {
    if (!class_exists('DatabaseTasks_mysql_async')) {
      // When changing driver from mysql to mysql_async, a registry
      // update might be needed. Until that happens, let's just require
      // the proper inc file.
      require_once DRUPAL_ROOT . '/includes/database/mysql_async/install.inc';
    }
    return 'mysql_async';
  }

  /**
   * Supply own default options if provided.
   *
   * @see DatabaseConnection::defaultOptions()
   */
  protected function defaultOptions() {
    return parent::defaultOptions() + $this->connectionOptions['default_options'];
  }

  /**
   * Perform an SQL query.
   *
   * @param mixed $query
   *   The query to perform.
   * @param array $args
   *   The arguments for the query.
   * @param array $options
   *   Options for the query.
   *     (optional) async: TRUE if an asynchronous query should be performed.
   *     (optional) async_key: The key for this query. Asynchronous
   *     queries using the same key will wait for each other to
   *     complete, thus reverting to running sequentially.
   *
   * @see DatabaseConnection::query()
   */
  public function query($query, array $args = array(), $options = array()) {
    $options += $this->defaultOptions();

    if (!$options['async']) {
      return $this->parentQuery($query, $args, $options);
    }
    if (!$options['async_transaction'] && $this->inTransaction()) {
      return $this->parentQuery($query, $args, $options);
    }
    else {
      try {
        // We allow either a pre-bound statement object or a literal string.
        // In either case, we want to end up with an executed statement object,
        // which we pass to PDOStatement::execute.
        if ($query instanceof DatabaseStatementAsync) {
          $stmt = $query;
          $stmt->execute($args, $options);
        }
        elseif ($query instanceof DatabaseStatementInterface) {
          // We cannot allow an already executed non-async query to become
          // async, because we cannot return the original database statement.
          $options['async'] = FALSE;
          return $this->parentQuery($query, $args, $options);
        }
        else {
          $key = isset($options['async_key']) ? $options['async_key'] : NULL;
          $dbhi = DatabaseConnectionAsync::allocate($this, $key);

          if (!$dbhi) {
            $options['async'] = FALSE;
            return $this->parentQuery($query, $args, $options);
          }

          $this->expandArguments($query, $args);
          $query = $this->prefixTables($query);
          $stmt = DatabaseStatementAsync::initialize($dbhi, $query);
          $stmt->execute($args, $options);
        }

        // Depending on the type of query we may need to return a different
        // value. See DatabaseConnection::defaultOptions() for a description of
        // each value.
        switch ($options['return']) {
          case Database::RETURN_STATEMENT:
            return $stmt;

          case Database::RETURN_AFFECTED:
            return $stmt->rowCount();

          case Database::RETURN_INSERT_ID:
            return $stmt->reapResult()->lastInsertId();

          case Database::RETURN_NULL:
            return;

          default:
            throw new MysqlAsyncException("Invalid return directive: '" . $options['return'] . "'", MysqlAsyncException::E_RETURN_UNSUPPORTED);
        }
      }
      catch (PDOException $e) {
        if ($options['throw_exception']) {
          // Add additional debug information.
          if ($query instanceof DatabaseStatementInterface) {
            $e->query_string = $stmt->getQueryString();
          }
          else {
            $e->query_string = $query;
          }
          $e->args = $args;
          throw $e;
        }
        return NULL;
      }
    }
  }

  /**
   * PDO::lastInsertId().
   *
   * We need to reap all async queries, in order to determine the last inserted
   * id, because the consumer of DatabaseConnection_mysql_async is unaware of
   * the multiple connections.
   */
  public function lastInsertId($seqname = NULL) {
    $results = DatabaseResultAsync::getAll();
    $last_insert_id = 0;

    // If $this->currentResultId is not set, then this means that the latest
    // query was performed on the root connection.
    if (!isset($this->currentResultId)) {
      $last_insert_id = parent::lastInsertId($seqname);
    }
    else {
      // Latest query is not on the root connection. Reap all queries and return
      // the last inserted id found.
      // @todo Is this safe to do in reverse, and then bail out early?
      foreach ($results as $result_id => $result) {
        $last_insert_id = $result->lastInsertId() ? $result->lastInsertId() : $last_insert_id;
      }
    }
    return $last_insert_id;
  }

  /**
   * Reap results on destroy.
   */
  public function __destruct() {
    $results = DatabaseResultAsync::getAll();
    foreach ($results as $result) {
      $result->reap();
    }
  }

}

/**
 * @} End of "addtogroup database".
 */
