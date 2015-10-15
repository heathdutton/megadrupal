<?php

/**
 * A subclass of QueueRunnerFSMBase for testing.
 */
class QueueRunnerFSMTest extends QueueRunnerFSMBase {

  protected $testVar;
  protected $default_transition;

  /**
   * Returns the state table that this FSM will execute.
   *
   * @return
   *   A string containing the human-readable version of the FSM.
   */
  protected function getStateTable() {
    return <<<EOT
# CurrentState           Transition  NextState              Wait  Attempts
  start                      *       taskOne
  taskOne                    *       taskTwo
  taskTwo                    *       waitForTaskTwo         1

  waitForTaskTwo             *       waitForTaskTwo         1       3
  waitForTaskTwo           fail      taskTwo

  waitForTaskTwo           done      taskThree

  taskThree                  *       finish
EOT;
  }

  public function simulateTaskSuccess() {
    $this->testVar = 'task succeeded';
  }

  public function simulateTaskFailure() {
    $this->testVar = 'task failed';
  }

  public function __construct($default_transition = 'processing') {
    // In this test class only affects waitForTaskTwo state.
    $this->default_transition = $default_transition;
  }

  // ----------------- FSM state methods ----------------------

  public function taskOne() {
    $this->results[] = __METHOD__;
    return $this->default_transition;
  }

  public function taskTwo() {
    $this->testVar = 'task started';
    $this->results[] = __METHOD__;
    return $this->default_transition;
  }

  public function waitForTaskTwo() {
    $this->results[] = __METHOD__;
    if ($this->testVar == 'task succeeded') {
      return 'done';
    }
    elseif ($this->testVar == 'task failed') {
      return 'fail';
    }
    return $this->default_transition;
  }

  public function taskThree() {
    $this->results[] = __METHOD__;
    return $this->default_transition;
  }

}