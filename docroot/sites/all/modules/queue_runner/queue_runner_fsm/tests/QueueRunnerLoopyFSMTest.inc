<?php

/**
 * A subclass of QueueRunnerFSMBase for testing.
 */
class QueueRunnerLoopyFSMTest extends QueueRunnerFSMBase {

  protected $desired_value = 0;
  protected $gnu = NULL;

  /**
   * Returns the state table that this FSM will execute.
   *
   * @return
   *   A string containing the human-readable version of the FSM.
   */
  protected function getStateTable() {
    return <<<EOT
# CurrentState       Transition  NextState              Wait  Attempts
  start                  *       createGnu
  createGnu              *       checkGnu
  checkGnu               *       retryGnu
  retryGnu               *       checkGnu
  checkGnu             done      finish
EOT;
  }


  public function __construct($desired_value = 1) {
    // Set the desired value of the gnu - equal to the number of retries we'll need.
    $this->desired_value = $desired_value;
  }

  // ----------------- FSM state methods ----------------------

  public function createGnu() {
    $this->results[] = __METHOD__;
    $this->gnu = new stdClass();
    // A fake value to test for doneness.
    $this->gnu->value = 0;
  }

  public function checkGnu() {
    $this->results[] = __METHOD__;
    if ($this->gnu->value == $this->desired_value) {
      return 'done';
    }
    return 'retry';
  }

  public function retryGnu() {
    $this->results[] = __METHOD__;
    $this->gnu->value++;
  }

}