<?php


Interface QueueRunnerFSMInterface {
  public function transition();
  public function getTransitionOutput();
  public function getCurrentState();
}

/**
 * The  is the base class of all objects.
 *
 * It implements a basic finite state machine.
 */
abstract class QueueRunnerFSMBase implements QueueRunnerFSMInterface {
  protected $task;
  protected $fsm = NULL;
  protected $current_state = 'start';
  protected $attempts = array();
  protected $next_transition_time = 0;
  protected $results = array();


  /**
   * Returns a default FSM.
   *
   * This method should be overridden to perform the actions you care
   * about.
   *
   * Format:
   * [CurrentState] [TransitionValue] [NewState] [Wait] [MaxAttempts]
   *
   * If Wait is 0, there will be no delay between the current state
   * and the new state.  If MaxAttempts is 0, the number of attempts
   * will be unlimited.
   *
   * The entry point to the FSM is 'start'.
   * The exit point is 'finish'.
   *
   * @return
   *   A string that contains a human-readable verison of the finite
   *   state machine.
   */
  protected function getStateTable() {
    return <<<EOT
# CurrentState Transition  NextState    Wait  Attempts
  start            *       falseStart    0     0
  falseStart       *       finish        0     0
EOT;
  }

  /**
   * A default state in the default FSM.
   *
   * Used to indicate the condition that the getStateTable was not
   * overridden in the subclass.
   *
   * @throw Exception
   *   This is thrown when the default state table has not been overridden.
   */
  protected function falseStart() {
    throw new Exception(sprintf('You must implement %s::getStateTable.', get_class($this)));
  }

  // ---------------------- FSM states ---------------------------

  /**
   * Always starts here.
   *
   * This is the default start state in the FSM graph.
   */
  protected function start() {
    return __FUNCTION__;
  }

  /**
   * Always end here.
   *
   * This is the default end state in the FSM graph.
   */
  protected function finish() {
    return __FUNCTION__;
  }

  public function getTransitionOutput() {
    return $this->results;
  }

  // ---------------------- FSM execution ---------------------

  /**
   * Causes the next transition in the finite state machine.
   *
   * Once configured, repeated calls to the transition method will
   * cause the appropriate checks and transitions to occur,
   * effectively running the finite state machine.
   *
   * @return
   *   TRUE if the FSM has more work to do; FALSE if it is finished.
   *
   * @throw WipTransitionMethodMissingException
   *   If a transition check method has not been implemented.
   * @throw WipMaximumTransitionsException
   *   If a path in the FSM has exceeded its maximum attempts value.
   */
  public function transition() {
    // Empty out prior results
    $this->results = array();
    // Make sure it is time to attempt the next transition.
    $time = time();
    if ($time < $this->next_transition_time) {
      // Not time yet, call back later.
      return TRUE;
    }

    $state = $this->current_state;
    if ($state === 'finish') {
      // Done.
      return FALSE;
    }
    if ($state === 'start' || empty($state)) {
      // start must always have an unconditional transition to the 1st real state.
      $newState = $this->findNewState('start', '*');
      $state = $newState->next;
      $this->current_state = $state;
    }
    // Perform the check and evaluate the result.
    if (method_exists($this, $state)) {
      $value = $this->$state();
    }
    else {
      throw new Exception("Method {$state} has not been implemented.");
    }

    $newState = $this->findNewState($state, $value);
    $next_state = $newState->next;

    // Enforce max attempts if specified.
    $transition_id = $this->getTransitionId($value, $newState);
    if ($newState->attempts > 0 &&
        $this->attempts[$transition_id] >= $newState->attempts) {
      // The maximum attempts has been exceeded.
      $message = sprintf("Maximum transitions for %s has been exceeded.", $transition_id);
      throw new Exception($message);
    }

    $this->current_state = $next_state;
    $this->attempts[$transition_id]++;

    $this->next_transition_time = $newState->wait + time();

    return ($this->current_state !== 'finish');
  }

  /**
   * Returns the current state in the finite state machine.
   *
   * @return
   *   A string containing the name of the current state.
   */
  public function getCurrentState() {
    return $this->current_state;
  }

  /**
   * Determines the new state to transition to.
   *
   * The current state and value from the transition check are used to
   * look up the next state in the finite state machine.
   *
   * @param $state_name
   *   The name of the current state.
   * @param $value
   *   The value returned from the transition check.
   * @return
   *   An object indicating the next state to transition to and the
   *   paramaters of that transition including maximum attempts and if
   *   there should be a delay before evaluating the next transition
   *   check.
   *
   * @throw WipTransitionNotDefinedException
   *   If a transition from the specified state with the specified
   *   value has not been defined.
   */
  protected function findNewState($state_name, $value) {
    $fsm = $this->getFSM();
    if (isset($fsm[$state_name][$value])) {
      $result = $fsm[$state_name][$value];
    }
    elseif (isset($fsm[$state_name]['*'])) {
      $result = $fsm[$state_name]['*'];
    }
    else {
      throw new Exception("The transition from state $state_name with value $value has not been defined in the finite state machine.");
    }
    return $result;
  }

  /**
   * Creates a unique transition value used for calculating attempt counts.
   *
   * This id uniquely represents a transition from one state to the next.
   *
   * @param $value
   *   The value from the transition check.
   * @param $state
   *   The state object from the finite state machine that describes
   *   the parameters of a particular transition.
   * @return
   *   A string id representing the transition.
   */
  protected function getTransitionId($value, $state) {
    $fsm = $this->getFSM();
    $simple_value = $value;
    if (!empty($fsm[$state->current][$value])) {
      $simple_value = $value;
    }
    else {
      // Simplify the value.  Any value not specified in the FSM will
      // be the generic transition value
      $simple_value = '*';
    }
    $id = sprintf('%s->%s->%s', $state->current, $simple_value, $state->next);

    // Now make sure there is an enry in the attempts field.
    if (!isset($this->attempts[$id])) {
      $this->attempts[$id] = 0;
    }
    return $id;
  }

  // --------------------- FSM compiler -----------------------

  /**
   * Returns the compiled finite state machine.
   *
   * If the FSM has not yet been compiled, it will be compiled and
   * verified as a result of invoking this method.
   *
   * @return
   *   The compiled FSM array.
   */
  protected function getFSM() {
    if (!isset($this->fsm)) {
      $fsm = $this->parseFSM($this->getStateTable());
      if ($this->verifyFSM($fsm)) {
        $this->fsm = $fsm;
      }
    }
    return $this->fsm;
  }

  /**
   * Parses the human readable state table and generates the fsm.
   *
   * The fsm is a complex structure that makes it possible to
   * determine the next state given the current state and a transition
   * value.
   *
   * @param $stateTable
   *   A string containing a human readable finite state machine definition.
   */
  protected function parseFSM($stateTable) {
    $fsm = array();
    $stateTable = preg_replace('/[ \t]+/', ' ', $stateTable);
    $transitions = explode("\n", $stateTable);

    foreach ($transitions as $transition) {
      $state = strtok($transition, ' ');
      if ($state === FALSE) {
        // strtok is finished
        continue;
      }
      if (strpos($state, '#') !== FALSE) {
        // The line is a comment.
        continue;
      }
      $transition_value = strtok(' ');
      $fsm[$state][$transition_value] = new stdClass();
      $fsm[$state][$transition_value]->current = $state;
      $fsm[$state][$transition_value]->next = strtok(' ');
      $wait = strtok(' ');
      $attempts = strtok(' ');

      // If the table doesn't include wait or attempts columns, add a
      // default value.
      if ($wait === FALSE) {
        $wait = 0;
      }
      $fsm[$state][$transition_value]->wait = (int)$wait;

      if ($attempts === FALSE) {
        $attempts = 0;
      }
      $fsm[$state][$transition_value]->attempts = (int)$attempts;
    }
    return $fsm;
  }

  /**
   * Verifies the finite state machine.
   *
   * Checks for missing methods or other bad values.
   *
   * @param $fsm
   *   The fsm strucure created by parseFSM.
   */
  protected function verifyFSM($fsm) {
    // Verify that we have methods for each state.
    foreach ($fsm as $state => $transitions) {
      if (!method_exists($this, $state)) {
        $this->errorMissingMethod($state);
      }
      foreach ($transitions as $transition_check => $transition) {
        if (!method_exists($this, $transition->next)) {
          $this->errorMissingMethod($transition->next);
        }
        if (!is_numeric($transition->wait)) {
          $this->errorNAN($state, $transition->next, 'wait', $transition->wait);
        }
        if (!is_numeric($transition->attempts)) {
          $this->errorNAN($state, $transition->next, 'attempts', $transition->attempts);
        }
      }
    }

    return TRUE;
  }

  /**
   * A state name in the state table does not have a corresponding method.
   *
   * @param $method
   *   The name of the method that should exist but doesn't.
   *
   * @throw WipMissingMethodException
   */
  protected function errorMissingMethod($method) {
    throw new Exception(sprintf('The implementation of %s must contain a method definition for the state "%s"', get_class($this), $method));
  }

  /**
   * A numeric field in the state table contains a non-numeric value.
   *
   * @param $state
   *   The name of the state that contains the bad value.
   * @param $next
   *   The name of the next state.
   * @param $key
   *   The name of the field that has a bad value ('wait' or 'attempts').
   * @param $value
   *   A string containing the bad value.
   *
   * @throw WipBadStateTableValueException
   */
  protected function errorNAN($state, $next, $key, $value) {
    throw new Exception(sprintf('The value of %s > %s for %s is "%s" but should be numeric.', $state, $next, $key, $value));
  }

}

