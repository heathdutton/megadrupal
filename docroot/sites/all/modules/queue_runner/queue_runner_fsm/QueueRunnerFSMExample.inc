<?php

/**
 * An example QueueRunnerFSMBase subclass.
 */
class QueueRunnerFSMExample extends QueueRunnerFSMBase {

  protected $created;

  public function __construct() {
    $this->created = time();
  }

  /**
   * Specifies the state table that this FSM will execute.
   *
   * @return
   *   A string containing the human-readable version of the FSM.
   */
  protected function getStateTable() {
    return <<<EOT
# CurrentState           Transition  NextState              Wait  Attempts
  start                      *       taskOne
  taskOne                    *       taskTwo
  taskTwo                    *       waitForTaskTwo         10

  waitForTaskTwo             *       waitForTaskTwo         10
  waitForTaskTwo           done      taskThree

  taskThree                  *       finish
EOT;
  }

  // ----------------- FSM state methods ----------------------

  public function taskOne() {
    $this->results[] = __METHOD__;
    return 'done';
  }

  public function taskTwo() {
    $this->results[] = __METHOD__;
    return 'done';
  }

  public function waitForTaskTwo() {
    $time_now = time();
    $diff = $time_now - $this->created;
    $this->results[] = __METHOD__ . " created: {$this->created} now: {$time_now} difference: {$diff}";
    if ($diff > 60) {
      return 'done';
    }
    return 'processing';
  }

  public function taskThree() {
    $this->results[] = __METHOD__;
    return 'done';
  }

}