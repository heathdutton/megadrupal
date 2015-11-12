<?php
/**
 * Created by PhpStorm.
 * User: wayne
 * Date: 10/10/14
 * Time: 11:26 PM
 */

class WdWrapperQueryResults implements Iterator {

  private $results;
  private $position;
  private $entityType;
  private $entityInfo;

  function __construct($entity_type, $results) {
    if (!empty($results[$entity_type])) {
      $this->results = array_values($results[$entity_type]);
    }
    else {
      $this->results = array();
    }
    $this->position = 0;
    $this->entityType = $entity_type;
    $this->entityInfo = entity_get_info($entity_type);
  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Return the current element
   * @link http://php.net/manual/en/iterator.current.php
   * @return WdEntityWrapper
   */
  public function current() {
    $current = $this->results[$this->position];
    $bundle_key = !empty($this->entityInfo['entity keys']['bundle']) ? $this->entityInfo['entity keys']['bundle'] : 'bundle';
    $bundle = !empty($current->{$bundle_key}) ? $current->{$bundle_key} : NULL;
    $class = wrappers_delight_get_wrapper_class($this->entityType, $bundle);
    if ($class == 'WdEntityWrapper') {
      return new WdEntityWrapper($this->entityType, $current->{$this->entityInfo['entity keys']['id']});
    }
    return new $class($current->{$this->entityInfo['entity keys']['id']});
  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Move forward to next element
   * @link http://php.net/manual/en/iterator.next.php
   * @return void Any returned value is ignored.
   */
  public function next() {
    ++$this->position;
  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Return the key of the current element
   * @link http://php.net/manual/en/iterator.key.php
   * @return mixed scalar on success, or null on failure.
   */
  public function key() {
    return $this->position;
  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Checks if current position is valid
   * @link http://php.net/manual/en/iterator.valid.php
   * @return boolean The return value will be casted to boolean and then evaluated.
   * Returns true on success or false on failure.
   */
  public function valid() {
    return isset($this->results[$this->position]);
  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Rewind the Iterator to the first element
   * @link http://php.net/manual/en/iterator.rewind.php
   * @return void Any returned value is ignored.
   */
  public function rewind() {
    $this->position = 0;
  }


}