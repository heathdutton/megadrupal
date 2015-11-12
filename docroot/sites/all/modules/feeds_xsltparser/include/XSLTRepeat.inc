<?php
/**
 * @file: Implements an index iterator over one XSLTPipe. The number of times
 * the pipe is repeated is determined by applying the counter pattern.
 */
class XSLTRepeat implements Iterator, XSLTPipe {
  private $idx;
  private $repetitions;

  /**
   * XSLTPipe which will be repeated for each key
   */
  private $pipe;

  /**
   * XSLTChain executed from within XSLTMap::run to determine the number times
   * the pipe should be ran.
   */
  private $counter;

  /**
   * Construct an XSLTRepeat instance.
   *
   * @param $counter  An XSLTChain returning an integer denoting the number of
   *                  repetitions
   * @param $pipe     The XSLTPipe which should be repeated.
   */
  public function __construct(XSLTChain $counter, XSLTPipe $pipe) {
    $this->counter = $counter;
    $this->pipe = $pipe;
  }

  /**
   * Implements Iterator::current().
   */
  public function current() {
    return $this->pipe;
  }

  /**
   * Implements Iterator::key().
   */
  public function key() {
    return $this->idx;
  }

  /**
   * Implements Iterator::next().
   */
  public function next() {
    $this->idx++;
  }

  /**
   * Implements Iterator::rewind().
   */
  public function rewind() {
    $this->idx = 0;
  }

  /**
   * Implements Iterator::valid().
   */
  public function valid() {
    return $this->idx < $this->repetitions;
  }


  /**
   * Implement XSLTPipe::run().
   */
  public function run($markup, $params = array()) {
    $result = trim($this->counter->run($markup, $params));
    if (!preg_match('/^\d+$/', $result)) {
      throw new Exception(t('Result of counter stylesheet must be a positive integer. Got @result instead.', array('@result' => $result)));
    }

    $this->repetitions = intval($result);
    return $result;
  }
}
