<?php
/**
 * @file
 * Log classes.
 */

/**
 * Wrapper for DatabaseStatement class used by DatabaseLog.
 */
class DatabaseLogAsyncStatement extends DatabaseStatementBase {
  protected $dbhi;
  protected $qs;
  protected $comment;
  public $dbh;

  /**
   * Constructor.
   *
   * Setup data used by DatabaseLog.
   */
  public function __construct($query_string, $dbhi, $comment = NULL) {
    $this->qs = $query_string;
    $this->dbhi = $dbhi;
    $this->dbh = $this;
    $this->comment = $comment;
  }

  /**
   * Safety net.
   *
   * We don't want parent destructors for this class called.
   */
  public function __destruct() {
  }

  /**
   * Dummy method so that we may call this class a DatabaseStatementInterface.
   */
  public function getIterator() {}

  /**
   * Return the target, including the async pool info.
   */
  public function getTarget() {
    return $this->dbhi->getConnection()->getTarget() . ':' . $this->dbhi->getKey();
  }

  /**
   * Return the query string.
   */
  public function getQueryString() {
    if ($this->comment) {
      return '/* ' . $this->comment . ' */ ' . $this->qs;
    }
    else {
      return $this->qs;
    }
  }

}
