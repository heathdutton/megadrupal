<?php
/**
 * @file
 * Asynchronous connection handler.
 */

/**
 * Class for handling database pool connections.
 */
class DatabaseConnectionPool {
  static protected $pool;

  protected $connections = array();
  protected $connection;
  protected $counter = 0;

  /**
   * Constructor.
   */
  public function __construct($connection) {
    $this->connection = $connection;
  }

  /**
   * Singleton factor.
   *
   * @param object $connection
   *   The DatabaseConnection object.
   *
   * @return DatabaseConnectionPool
   *   The pool object.
   */
  static public function factory($connection) {
    $key = $connection->getKey();
    $target = $connection->getTarget();
    if (empty(self::$pool[$key][$target])) {
      self::$pool[$key][$target] = new self($connection);
    }
    return self::$pool[$key][$target];
  }

  /**
   * Generate real key.
   *
   * @param string $key
   *   The key.
   *
   * @return string
   *   The key.
   */
  public function realKey($key) {
    return $key ? "key:$key" : "pool:" . ($this->counter);
  }

  /**
   * Get a connection from the pool.
   *
   * @param string $key
   *   The key to use.
   *
   * @return mixed
   *   DatabaseConnectionPool object or FALSE.
   */
  public function getConnection($key) {
    $realkey = $this->realKey($key);

    // Is the connection in use?
    if (!empty($this->connections[$realkey])) {
      // Yes, is it available?
      if ($this->connections[$realkey]['available']) {
        DatabaseConnectionAsync::log("Reusing free keyed: $realkey");
        // Yes, let's use it.
        $this->connections[$realkey]['available'] = FALSE;
        return $this->connections[$realkey]['connection'];
      }
      // No, let's reap it.
      if ($this->connections[$realkey]['connection']->reap()) {
        DatabaseConnectionAsync::log("Reusing reaped: $realkey");
        // Reaped, now we can use it.
        $this->connections[$realkey]['available'] = FALSE;
        return $this->connections[$realkey]['connection'];
      }
      // Sorry, connection not available.
      throw new MysqlAsyncException("Could not re-use connection for key $key", MysqlAsyncException::E_KEY_REUSE);
    }

    // Connection not in use, let's get a free one if there is one.
    foreach ($this->connections as $oldkey => $connection) {
      if ($connection['available']) {
        DatabaseConnectionAsync::log("Reusing free: $oldkey => $realkey");
        unset($this->connections[$oldkey]);
        $this->connections[$realkey] = array(
          'available' => FALSE,
          'connection' => $connection['connection'],
        );
        $connection['connection']->reInitialize($realkey);
        // Bump pool counter if necessary.
        if (!isset($key)) {
          $this->counter++;
        }
        return $this->connections[$realkey]['connection'];
      }
    }
    $connection_options = $this->connection->getConnectionOptions();

    // No free connections. Let's "soft-reap" open connections.
    if ($connection_options['pre_soft_reap']) {
      foreach ($this->connections as $oldkey => $connection) {
        if ($connection['connection']->reap(0)) {
          DatabaseConnectionAsync::log("Reusing soft-reaped: $oldkey => $realkey");
          unset($this->connections[$oldkey]);
          $this->connections[$realkey] = array(
            'available' => FALSE,
            'connection' => $connection['connection'],
          );
          $connection['connection']->reInitialize($realkey);
          return $this->connections[$realkey]['connection'];
        }
      }
    }

    // Nothing reaped. Do we have room for opening a new one?
    if (count($this->connections) < $connection_options['async_max_connections']) {
      // "Plenty of room". Let's create a new connection.
      if ($mysqli = $this->openConnection()) {
        DatabaseConnectionAsync::log("New connection for: $realkey");
        $this->connections[$realkey] = array(
          'available' => FALSE,
          'connection' => new DatabaseConnectionAsync($this->connection, $mysqli, $realkey, $this),
        );
        // Bump pool counter if necessary.
        if (!isset($key)) {
          $this->counter++;
        }
        return $this->connections[$realkey]['connection'];
      }
      // Could not create a new connection. Let's continue reaping an old
      // connection as fallback.
    }

    // No free connections. Let's "soft-reap" open connections.
    if (!$connection_options['pre_soft_reap']) {
      foreach ($this->connections as $oldkey => $connection) {
        if ($connection['connection']->reap(0)) {
          DatabaseConnectionAsync::log("Reusing soft-reaped: $oldkey => $realkey");
          unset($this->connections[$oldkey]);
          $this->connections[$realkey] = array(
            'available' => FALSE,
            'connection' => $connection['connection'],
          );
          $connection['connection']->reInitialize($realkey);
          return $this->connections[$realkey]['connection'];
        }
      }
    }

    // No room left in pool. Let's reap an old connection.
    foreach ($this->connections as $oldkey => $connection) {
      if ($connection['connection']->reap()) {
        DatabaseConnectionAsync::log("Reusing hard-reaped: $oldkey => $realkey");
        unset($this->connections[$oldkey]);
        $this->connections[$realkey] = array(
          'available' => FALSE,
          'connection' => $connection,
        );
        return $this->connections[$realkey]['connection'];
      }
    }

    // Not possible to acquire a connection.
    DatabaseConnectionAsync::log("No connection found for: $realkey");
    return FALSE;
  }

  /**
   * Open an asynchronous database connection.
   *
   * @return DatabaseConnectionAsync
   *   An asynchronous database connection object.
   */
  public function openConnection() {
    $connection_options = $this->connection->getConnectionOptions();

    $mysqli = new mysqli();

    if (isset($connection_options['mysqli']['pdo'][PDO::ATTR_TIMEOUT])) {
      $mysqli->options(MYSQLI_OPT_CONNECT_TIMEOUT, $connection_options['mysqli']['pdo'][PDO::ATTR_TIMEOUT]);
    }

    $result = $mysqli->real_connect(
      $connection_options['mysqli']['host'],
      $connection_options['mysqli']['username'],
      $connection_options['mysqli']['password'],
      $connection_options['mysqli']['database'],
      $connection_options['mysqli']['port'],
      $connection_options['mysqli']['socket']
    );
    if (!$result) {
      return FALSE;
    }

    // Force MySQL to use the UTF-8 character set. Also set the collation, if a
    // certain one has been set; otherwise, MySQL defaults to 'utf8_general_ci'
    // for UTF-8.
    if (!empty($connection_options['mysqli']['collation'])) {
      $mysqli->query('SET NAMES utf8 COLLATE ' . $connection_options['mysqli']['collation']);
    }
    else {
      $mysqli->query('SET NAMES utf8');
    }

    // Run init_commands.
    if ($connection_options['mysqli']['init_commands']) {
      $mysqli->multi_query(implode('; ', $connection_options['mysqli']['init_commands']));
      do {
        $result = $mysqli->use_result();
        if (is_object($result)) {
          $result->free();
        }
      } while ($mysqli->more_results() && $mysqli->next_result());
    }

    return $mysqli;
  }

  /**
   * Free a connection.
   *
   * @param string $realkey
   *   The real key.
   */
  public function free($realkey) {
    $this->connections[$realkey]['available'] = TRUE;
  }

  /**
   * Remove connection from pool.
   *
   * @param string $realkey
   *   The real key.
   */
  public function remove($realkey) {
    unset($this->connections[$realkey]);
  }

}

/**
 * Wrapper for bundling a DBTNG and a mysqli connection.
 */
class DatabaseConnectionAsync {
  protected $key;
  protected $connection;
  protected $mysqli;

  protected $queryString;
  protected $queryArgs;
  protected $unparsedQueryString;

  protected $pool;

  protected $timeout = 60;
  public $resultId;

  /**
   * Get current connection key.
   *
   * @return string
   *   The current connection key.
   */
  public function getKey() {
    return $this->key;
  }

  /**
   * For debugging purposes.
   */
  static public function log($msg) {
    if (!empty($GLOBALS['conf']['mysql_async_debug_log'])) {
      error_log("$msg");
    }
  }


  /**
   * Factory method for getting a connection.
   *
   * @param object $connection
   *   Database connection object.
   * @param string $key
   *   (optional) Async key.
   *
   * @return mixed
   *   MysqlAsyncConnection object on succes, FALSE if not.
   */
  public static function allocate($connection, $key = NULL) {
    $pool = DatabaseConnectionPool::factory($connection);

    if ($conn = $pool->getConnection($key)) {
      return $conn;
    }
    return FALSE;
  }

  /**
   * Reap all results.
   */
  public function reap($timeout = NULL) {
    self::log("DBHI reaping: $this->resultId");
    if ($result = DatabaseResultAsync::get($this->resultId)) {
      return $result->reap($timeout);
    }
  }

  /**
   * Constructor. Setup initial conditions.
   *
   * @param object $connection
   *   Database connection object.
   * @param object $mysqli
   *   mysqli object.
   * @param string $key
   *   Async key.
   */
  public function __construct($connection, $mysqli, $key, $pool) {
    $this->connection = $connection;
    $this->mysqli = $mysqli;
    $this->key = $key;
    $this->pool = $pool;
  }

  /**
   * Destructor.
   */
  public function __destruct() {
    self::log("DESTROYING: $this->key");
  }

  /**
   * Initialize a connection for re-use.
   *
   * @param string $key
   *   New key to use.
   */
  public function reInitialize($key) {
    $this->key = $key;
    return $this;
  }

  /**
   * Set query timeout for this connection.
   *
   * @param int $timeout
   *   Timeout in seconds.
   */
  public function setTimeout($timeout) {
    $this->timeout = $timeout;
  }

  /**
   * Free up a connection in the pool.
   *
   * Closes the connection if pool is "overallocated"
   * (async_min_connections).
   */
  public function free() {
    $this->pool->free($this->key);
  }

  /**
   * Close the mysqli connection.
   */
  public function close() {
    self::log("Closing: $this->key");
    $this->pool->remove($this->key);
    return @$this->mysqli->close();
  }

  /**
   * Get DBTNG connection.
   *
   * @return object
   *   Database connection object.
   */
  public function getConnection() {
    return $this->connection;
  }

  /**
   * Get mysqli connection.
   *
   * @return mysqli
   *   mysqli connection object.
   */
  public function query($query, $args, $unparsed_query) {
    self::log("Query: $query");
    $this->queryString = $query;
    $this->queryArgs = $args;
    $this->unparsedQueryString = $unparsed_query;
    $result = $this->mysqli->query($query, MYSQLI_ASYNC | MYSQLI_STORE_RESULT);
    $this->started = microtime(TRUE);
    return $result;
  }

  /**
   * Poll result.
   *
   * If result is ready, then free up connection.
   *
   * @param float $timeout
   *   (optional) Maximum number of seconds to poll, before giving up.
   *              If not specified, the current connection's timeout will be
   *              used.
   *
   * @return mysqliresult
   *   mysqliresult object.
   */
  public function poll($timeout = NULL) {
    $timeout = isset($timeout) ? $timeout : $this->timeout - (microtime(TRUE) - $this->started);

    if ($timeout < 0) {
      $e = new MysqlAsyncException("Asynchronous query exceeded time limit of $this->timeout seconds", MysqlAsyncException::E_TIMEOUT);
      $e->query_string = $this->queryString;
      $e->args = $this->queryArgs;
      throw $e;
    }

    $links = array($this->mysqli);
    $error = $reject = array();
    self::log("Polling ($timeout): $this->queryString - " . date('H:i:s', $this->started));
    $started = microtime(TRUE);
    if ($result = @mysqli::poll($links, $error, $reject, 0, $timeout * 1000000)) {
      $result = $this->mysqli->reap_async_query();

      $this->logQuery('REAPED', microtime(TRUE) - $started);

      if ($this->mysqli->errno) {
        $msg = $this->mysqli->errno . ' ' . $this->mysqli->error;
        $e = new PDOException($msg, $this->mysqli->errno);
        $e->query_string = $this->unparsedQueryString;
        $e->args = $this->queryArgs;
        throw $e;
      }

      if (!$result) {
        $e = new MysqlAsyncException("Asynchronous query exceeded time limit of $this->timeout seconds", MysqlAsyncException::E_TIMEOUT);
        $e->query_string = $this->queryString;
        $e->args = $this->queryArgs;
        throw $e;
      }

      // Provide write results to result object before freeing.
      if ($aresult = DatabaseResultAsync::get($this->resultId)) {
        $aresult->injectResult(
          $result,
          $this->mysqli->insert_id,
          $this->mysqli->affected_rows
        );
      }

      $this->free();
      return $result;
    }
    if ($timeout) {
      $e = new MysqlAsyncException("Asynchronous query exceeded time limit of $this->timeout seconds", MysqlAsyncException::E_TIMEOUT);
      $e->query_string = $this->queryString;
      $e->args = $this->queryArgs;
      throw $e;
    }
  }

  /**
   * Log query.
   *
   * @param string $msg
   *   Message to log.
   * @param float $duration
   *   Duration of the query.
   */
  public function logQuery($msg = NULL, $duration = NULL) {
    if ($logger = $this->connection->getLogger()) {
      $duration = isset($duration) ? $duration : microtime(TRUE) - $this->started;
      $logger->log(new DatabaseLogAsyncStatement($this->unparsedQueryString, $this, $msg), $this->queryArgs, $duration);
    }
  }

  /**
   * Get last insert id.
   */
  public function lastInsertId() {
    return $this->mysqli->insert_id;
  }

  /**
   * Get affected rows.
   */
  public function affectedRows() {
    return $this->mysqli->affected_rows;
  }

}
