<?php
/**
 * @file
 * Iterator class for database statements.
 */

/**
 * Result iterator for database statements.
 */
class DatabaseStatementAsyncIterator implements Iterator {
  protected $pos = 0;
  protected $current = NULL;
  protected $stmt = NULL;

  /**
   * Constructor.
   *
   * @param DatabaseStatementInterface $stmt
   *   The statement to iterate over.
   */
  public function __construct(DatabaseStatementInterface $stmt) {
    $this->stmt = $stmt;
  }

  /**
   * Destructor.
   */
  public function __destruct() {
    DatabaseConnectionAsync::log("DESTROYING iterator: {$this->stmt->resultId}");

  }

  /**
   * Iterator::current().
   */
  public function current() {
    return $this->current;
  }

  /**
   * Iterator::key().
   */
  public function key() {
    return $this->pos;
  }

  /**
   * Iterator::next().
   */
  public function next() {
    $this->pos++;
    $this->current = $this->stmt->fetch();
  }

  /**
   * Iterator::rewind().
   */
  public function rewind() {
    $this->pos = 0;
    // By disallowing rewind, we maintain iterator compatibility with PDO.
    // $this->stmt->result->data_seek($this->pos);
    $this->current = $this->stmt->fetch();
  }

  /**
   * Iterator::valid().
   */
  public function valid() {
    return $this->pos < $this->stmt->rowCount();
  }

}
