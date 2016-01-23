<?php
/**
 * @file
 * Class for database statements.
 */

/**
 * Pseudo PDOStatement class for DBTNG.
 *
 * This statement class is used for the asynchronous queries performed.
 * PDO does not support asynchronous queries, so we wrap up ye olde
 * mysqli objects, and make them look PDO-ish.
 */
class DatabaseStatementAsync implements IteratorAggregate, DatabaseStatementInterface {

  protected $queryString;
  protected $queryArgs;
  protected $queryOptions;
  protected $dbhiKey;

  protected $dbhi;
  protected $fetchMode = PDO::FETCH_BOTH;
  protected $fetchModeArgs = array();
  protected $query;
  public $resultId;


  /**
   * Factory for asynchronous pdo statement class.
   */
  static public function initialize($dbhi, $query) {
    $obj = new self();
    $obj->dbhi = $dbhi;
    $obj->dbhiKey = $dbhi->getKey();
    $obj->dbh = $dbhi->getConnection();
    $obj->queryString = $query;
    return $obj;
  }

  /**
   * IteratorAggregate::getIterator().
   */
  public function getIterator() {
    return new DatabaseStatementAsyncIterator($this);
  }

  /**
   * PDO::execute().
   */
  public function execute($args = array(), $options = array()) {
    // Free existing result, if any, before executing.
    if (isset($this->resultId)) {
      // Free if applicable.
      DatabaseResultAsync::free($this->resultId);
    }

    // DatabaseConnection::query() might call us with NULL args.
    // In this case, it's a re-execution of an existing statement.
    if (!isset($args)) {
      $args = $this->queryArgs;
    }

    $this->queryArgs = $args;
    $this->queryOptions = $options;

    if (!$this->dbhi) {
      if (!($this->dbhi = DatabaseConnectionAsync::allocate($this, $this->key))) {
        return $this->dbh->parentQuery($this->queryString, $args, $options);
      }
    }

    // Set timeout.
    $this->dbhi->setTimeout($options['async_timeout']);

    // Pre-allocate result object.
    $this->resultId = DatabaseResultAsync::allocate($this->dbhi);

    if (isset($options['fetch'])) {
      if (is_string($options['fetch'])) {
        // Default to an object. Note: db fields will be added to the object
        // before the constructor is run. If you need to assign fields after
        // the constructor is run, see http://drupal.org/node/315092.
        $this->setFetchMode(PDO::FETCH_CLASS, $options['fetch']);
      }
      else {
        $this->setFetchMode($options['fetch']);
      }
    }

    $query = $this->queryString;

    if ($args) {
      $this->replaceQueryArguments($query, $args);
    }

    $query_start = microtime(TRUE);
    $return = $this->dbhi->query($query, $args, $this->queryString);
    $this->dbhi->logQuery('INITIATED', microtime(TRUE) - $query_start);

    return $return;
  }

  /**
   * Process query placeholders.
   *
   * @param string &$query
   *   The query containing placeholders.
   * @param array $args
   *   The values for the placeholders.
   */
  protected function replaceQueryArguments(&$query, array $args) {
    foreach ($args as $key => $value) {
      if (!$this->replaceQueryArgument($query, $key, $value)) {
        throw new RuntimeException("Unbound parameter: $key");
      }
    }
  }

  /**
   * Process a single query placeholders.
   *
   * @param string &$query
   *   The query containing placeholders.
   * @param string $search
   *   The placeholder.
   * @param string $replace
   *   The value to replace the placeholder with.
   */
  protected function replaceQueryArgument(&$query, $search, $replace) {
    if (is_numeric($search)) {
      $search = '?';
    }
    elseif (strpos($search, ':') !== 0) {
      $search = ":$search";
    }

    if (is_array($replace)) {
      foreach ($replace as &$val) {
        $val = $this->dbh->quote($val);
      }
      $replace = implode(',', $replace);
    }
    else {
      $replace = $this->dbh->quote($replace);
    }
    $count = 0;
    $regex = '/(^|[\W])' . preg_quote($search) . '(\W|$)/';
    $subst = '$1' . preg_quote($replace) . '$2';
    $query = preg_replace($regex, $subst, $query, 1, $count);
    return $count;
  }

  /**
   * PDO::rowCount().
   *
   * @param bool $poll_only
   *   (optional) Poll only (i.e. don't wait for result). Defaults to FALSE.
   *
   * @return mixed
   *   Number of rows if async query is finished, otherwise NULL.
   */
  public function rowCount() {
    $args = func_get_args();
    if (empty($args[0])) {
      try {
        $this->reapResult();
      }
      catch (PDOException $e) {
        if ($this->queryOptions['throw_exception']) {
          // Add additional debug information.
          $e->query_string = $this->queryString;
          $e->args = $this->queryArgs;
          throw $e;
        }
        return NULL;
      }
    }
    return $this->getResult() ? $this->getResult()->rowCount() : NULL;
  }

  /**
   * PDO::fetch().
   */
  public function fetch($mode = NULL) {
    $mode = isset($mode) ? $mode : $this->fetchMode;

    try {
      switch ($mode) {
        case PDO::FETCH_ASSOC:
          return $this->reapResult()->fetch_assoc();

        case PDO::FETCH_BOTH:
          return $this->reapResult()->fetch_array(MYSQLI_BOTH);

        case PDO::FETCH_CLASS:
          return $this->fetchObject();

        case PDO::FETCH_OBJ:
          return $this->fetchObject('stdClass');

        case PDO::FETCH_NUM:
          return $this->reapResult()->fetch_row();

        case PDO::FETCH_COLUMN:
          $index = isset($this->fetchModeArgs[0]) ? $this->fetchModeArgs[0] : 0;
          return $this->fetchColumn($index);

        case PDO::FETCH_INTO:
          $result = $this->reapResult()->fetch_assoc();
          foreach ($result as $key => $value) {
            $this->fetchModeArgs[0]->$key = $value;
          }
          return $this->fetchModeArgs[0];

        case PDO::FETCH_BOUND:
          throw new MysqlAsyncException("PDO::FETCH_BOUND not supported", MysqlAsyncException::E_MODE_UNSUPPORTED);

        case PDO::FETCH_LAZY:
          throw new MysqlAsyncException("PDO::FETCH_LAZY not supported", MysqlAsyncException::E_MODE_UNSUPPORTED);

        case PDO::FETCH_NAMED:
          throw new MysqlAsyncException("PDO::FETCH_NAMED not supported", MysqlAsyncException::E_MODE_UNSUPPORTED);

        default:
          throw new MysqlAsyncException("Mode '$mode' not supported", MysqlAsyncException::E_MODE_UNSUPPORTED);

      }
    }
    catch (PDOException $e) {
      if ($this->queryOptions['throw_exception']) {
        // Add additional debug information.
        $e->query_string = $this->queryString;
        $e->args = $this->queryArgs;
        throw $e;
      }
      return NULL;
    }
  }

  /**
   * PDO::fetchAll().
   */
  public function fetchAll($mode = NULL, $column_index = NULL, array $constructor_arguments = array()) {
    $mode = isset($mode) ? $mode : $this->fetchMode;
    $result = array();
    try {
      if ($mode === PDO::FETCH_ASSOC) {
        $result = $this->reapResult()->fetch_all(MYSQLI_ASSOC | MYSQLI_STORE_RESULT);
      }
      else {
        // Set fetch mode, and use iterator for collecting data.
        $this->setFetchMode($mode, $column_index, $constructor_arguments);
        foreach ($this as $key => $value) {
          $result[$key] = $value;
        }
      }
    }
    catch (PDOException $e) {
      if ($this->queryOptions['throw_exception']) {
        // Add additional debug information.
        $e->query_string = $this->queryString;
        $e->args = $this->queryArgs;
        throw $e;
      }
      return array();
    }

    return $result;
  }

  /**
   * PDO::fetchColumn().
   */
  public function fetchColumn($index = 0) {
    try {
      $row = $this->reapResult()->fetch_array(MYSQLI_NUM);
      return $row ? $row[$index] : FALSE;
    }
    catch (PDOException $e) {
      if ($this->queryOptions['throw_exception']) {
        // Add additional debug information.
        $e->query_string = $this->queryString;
        $e->args = $this->queryArgs;
        throw $e;
      }
      return NULL;
    }
  }

  /**
   * PDO::fetchObject().
   */
  public function fetchObject() {
    // Determine fetch mode.
    $args = func_get_args();
    $default_class_name = isset($this->fetchModeArgs[0]) && $this->fetchMode === PDO::FETCH_CLASS ? $this->fetchModeArgs[0] : 'stdClass';
    $default_ctor_args = isset($this->fetchModeArgs[1]) && $this->fetchMode === PDO::FETCH_CLASS ? $this->fetchModeArgs[1] : NULL;

    $class_name = count($args) >= 1 ? $args[0] : $default_class_name;
    $ctor_args = count($args) >= 2 ? $args[1] : (empty($args) ? $default_ctor_args : NULL);

    try {
      return $this->reapResult()->fetch_object($class_name, $ctor_args);
    }
    catch (PDOException $e) {
      if ($this->queryOptions['throw_exception']) {
        // Add additional debug information.
        $e->query_string = $this->queryString;
        $e->args = $this->queryArgs;
        throw $e;
      }
      return NULL;
    }
  }

  /**
   * PDO::setFetchMode().
   */
  public function setFetchMode() {
    $args = func_get_args();
    $this->fetchMode = array_shift($args);
    $this->fetchModeArgs = $args;
  }

  /**
   * PDO::getFetchMode().
   */
  public function getFetchMode($fetch_mode) {
    return $this->fetchMode;
  }


  /**
   * DatabaseStatementInterface::getQueryString().
   *
   * @see DatabaseStatementBase::getQueryString()
   */
  public function getQueryString() {
    return $this->queryString;
  }

  /**
   * DatabaseStatementInterface::fetchCol().
   *
   * @see DatabaseStatementBase::fetchCol()
   */
  public function fetchCol($index = 0) {
    return $this->fetchAll(PDO::FETCH_COLUMN, $index);
  }

  /**
   * DatabaseStatementInterface::fetchAllAssoc().
   *
   * @see DatabaseStatementBase::fetchAllAssoc()
   */
  public function fetchAllAssoc($key, $fetch = NULL) {
    $return = array();
    if (isset($fetch)) {
      if (is_string($fetch)) {
        $this->setFetchMode(PDO::FETCH_CLASS, $fetch);
      }
      else {
        $this->setFetchMode($fetch);
      }
    }

    foreach ($this as $record) {
      $record_key = is_object($record) ? $record->$key : $record[$key];
      $return[$record_key] = $record;
    }

    return $return;
  }

  /**
   * DatabaseStatementInterface::fetchAllKeyed().
   *
   * @see DatabaseStatementBase::fetchAllKeyed()
   */
  public function fetchAllKeyed($key_index = 0, $value_index = 1) {
    $return = array();
    $this->setFetchMode(PDO::FETCH_NUM);
    foreach ($this as $record) {
      $return[$record[$key_index]] = $record[$value_index];
    }
    return $return;
  }

  /**
   * DatabaseStatementInterface::fetchField().
   *
   * @see DatabaseStatementBase::fetchField()
   */
  public function fetchField($index = 0) {
    // Call PDOStatement::fetchColumn to fetch the field.
    return $this->fetchColumn($index);
  }

  /**
   * DatabaseStatementInterface::fetchAssoc().
   *
   * @see DatabaseStatementBase::fetchAssoc()
   */
  public function fetchAssoc() {
    // Call PDOStatement::fetch to fetch the row.
    return $this->fetch(PDO::FETCH_ASSOC);
  }

  /**
   * Free up result when object dies.
   */
  public function __destruct() {
    DatabaseConnectionAsync::log("Destroying statement: $this->resultId");

    try {
      // Poll one last time for memory release.
      if (!$this->getResult()) {
        $this->reapResult(0);
      }
      if (!$this->getResult()) {
        $this->dbhi->close();
      }
    }
    catch (Exception $e) {
      // Clean up. Disregard exceptions.
    }
    DatabaseResultAsync::free($this->resultId);
  }

  /**
   * Capture unknown method calls.
   */
  public function __call($name, $args) {
    throw new MysqlAsyncException("Method '$name' not supported", MysqlAsyncException::E_METHOD_UNSUPPORTED);
  }

  /**
   * Capture unknown static function calls.
   */
  public static function __callStatic($name, $args) {
    throw new MysqlAsyncException("Static function '$name' not supported", MysqlAsyncException::E_METHOD_UNSUPPORTED);
  }

  /**
   * Get result if available.
   */
  public function getResult() {
    $result = DatabaseResultAsync::get($this->resultId);
    if ($result && $result->isReaped()) {
      return $result;
    }
  }

  /**
   * Reap and get result.
   *
   * @param float $timeout
   *   (optional) Timeout in seconds to poll for.
   */
  public function reapResult($timeout = NULL) {
    $result = DatabaseResultAsync::get($this->resultId);
    if (!$result) {
      return NULL;
    }
    if ($result && $result->reap($timeout)) {
      return $result;
    }
  }

}
