<?php
/**
 * @file
 * Result class.
 */

/**
 * Result class.
 */
class DatabaseResultAsync {
  protected $result;
  protected $dbhi;
  protected $reaped = FALSE;
  protected $freed = FALSE;
  protected $id;
  protected static $counter = 0;
  protected static $pool = array();

  /**
   * Constructor.
   *
   * @param DatabaseConnectionAsync $dbhi
   *   DB wrapper object.
   */
  public function __construct(DatabaseConnectionAsync $dbhi) {
    $this->id = self::$counter++;
    DatabaseConnectionAsync::log("Constructing result: $this->id");
    $this->dbhi = $dbhi;
    $this->dbhi->getConnection()->currentResultId = $this->dbhi->resultId = $this->id;
    self::$pool[$this->id] = $this;
  }

  /**
   * Allocate new result object.
   *
   * @param DatabaseConnectionAsync $dbhi
   *   Async database connection object.
   */
  public static function allocate(DatabaseConnectionAsync $dbhi) {
    $obj = new self($dbhi);
    return $obj->id;
  }

  /**
   * Free result.
   *
   * @param int $id
   *   The id to free.
   */
  public static function free($id) {
    if (isset(self::$pool) && isset(self::$pool[$id])) {
      self::$pool[$id]->close();
      unset(self::$pool[$id]);
    }
  }

  /**
   * Get result.
   *
   * @param int $id
   *   The id of the result.
   */
  public static function get($id) {
    return isset(self::$pool) && !empty(self::$pool[$id]) ? self::$pool[$id] : NULL;
  }

  /**
   * Get all results.
   */
  public static function getAll() {
    return isset(self::$pool) ? self::$pool : array();
  }

  /**
   * Reap result.
   */
  public function reap($timeout = NULL) {
    if (!$this->reaped) {
      DatabaseConnectionAsync::log("Trying to reap: $this->id");
      $this->dbhi->poll($timeout);
    }
    return $this->reaped;
  }

  /**
   * Inject result into this object.
   *
   * @param object $result
   *   MySQLi result object.
   * @param int $insert_id
   *   Insert id of query.
   * @param int $affected_rows
   *   Affected rows of query.
   */
  public function injectResult($result, $insert_id, $affected_rows) {
    DatabaseConnectionAsync::log("Reaped: $this->id");
    $this->reaped = TRUE;
    $this->result = $result;
    $this->insertId = $insert_id;
    $this->affectedRows = $affected_rows;
  }

  /**
   * Check if result has been reaped.
   */
  public function isReaped() {
    return $this->reaped;
  }

  /**
   * Return last insert id.
   */
  public function lastInsertId() {
    $this->reap();
    return $this->insertId;
  }

  /**
   * Return affected rows.
   */
  public function affectedRows() {
    $this->reap();
    return $this->affectedRows;
  }

  /**
   * Free up result when object dies.
   */
  public function __destruct() {
    DatabaseConnectionAsync::log("Destroying result: $this->id");
    self::free($this->id);
  }

  /**
   * Close up.
   */
  public function close() {
    DatabaseConnectionAsync::log("Freeing result: $this->id");
    $this->freed = TRUE;
    if (!$this->reaped) {
      $this->dbhi->logQuery('NEVER REAPED', 0);
    }
    if (is_object($this->result)) {
      $this->result->free();
    }
  }

  /**
   * Return rows fetched or rows affected.
   */
  public function rowCount() {
    $this->reap();
    return is_object($this->result) ? $this->result->num_rows : $this->affectedRows;
  }

  /**
   * Wrapper for data_seek().
   *
   * @see mysqli_data_seek()
   */
  public function data_seek($offset) {
    $this->reap();
    return $this->result->data_seek($offset);
  }

  /**
   * Wrapper for fetch_all().
   *
   * @see mysqli_fetch_all()
   */
  public function fetch_all($resulttype = MYSQLI_NUM) {
    $this->reap();
    return $this->result->fetch_all($resulttype);
  }

  /**
   * Wrapper for fetch_array().
   *
   * @see mysqli_fetch_array()
   */
  public function fetch_array($resulttype = MYSQLI_BOTH) {
    $this->reap();
    return $this->result->fetch_array($resulttype);
  }

  /**
   * Wrapper for fetch_assoc().
   *
   * @see mysqli_fetch_assoc()
   */
  public function fetch_assoc() {
    $this->reap();
    return $this->result->fetch_assoc();
  }

  /**
   * Wrapper for fetch_object().
   *
   * @see mysqli_fetch_object()
   */
  public function fetch_object($class_name = 'stdClass', $params = NULL) {
    $this->reap();
    // Cannot send constructor arguments to e.g. stdClass.
    if (isset($params)) {
      return $this->result->fetch_object($class_name, $params);
    }
    else {
      return $this->result->fetch_object($class_name);
    }
  }

  /**
   * Wrapper for fetch_row().
   *
   * @see mysqli_fetch_row()
   */
  public function fetch_row() {
    $this->reap();
    return $this->result->fetch_row();
  }

}
