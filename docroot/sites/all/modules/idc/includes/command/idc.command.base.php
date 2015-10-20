<?php
/**
 * @file
 * Drush Interactive Commands command base class.
 */

/**
 * Class IDCCommandBase
 */
abstract class IDCCommandBase implements IDCInterface {

  /**
   * @var string The name of the IDC command.
   */
  public static $commandName;

  /**
   * @var array The steps of the IDC command.
   */
  public $commandSteps;

  /**
   * @var array Additional (optional) info about the IDC command steps.
   */
  public $stepsInfo;

  /**
   * @var int Index of the current step being executed.
   */
  public $currentStep;

  /**
   * @var string The name of the next step method.
   */
  private $nextStep;

  /**
   * @var array The results stored for all the steps executed so far.
   */
  public $stepResults;

  /**
   * @var bool Flag to indicate whether the execution should be terminated.
   */
  public $terminated;

  /**
   * Constructs a new IDC command object, setting the inital value of variables.
   */
  public function __construct() {
    foreach ($this->getCommandSteps() as $key => $commandStep) {
      // Integer key means the step points to method in the command class.
      if (is_int($key)) {
        $this->commandSteps[] = $commandStep;
      }
      // String key means there's a step name as the key, and the value points
      // to something else. Class currently supports instances of IDCStepBase as
      // the value of getCommandSteps() returned items. If one of them is found,
      // store the actual step object in the stepsInfo property, indexed by step
      // name.
      else if (is_string($key)) {
        $this->commandSteps[] = $key;
        if ($commandStep instanceof IDCStepBase) {
          $this->stepsInfo[$key] = array(
            'step class' => $commandStep,
          );
        }
      }
    }

    $this->currentStep = 0;
    $this->terminated = FALSE;
  }

  /**
   * Returns the name of the IDC command.
   *
   * @return string
   *   The name of the drush command.
   */
  public static function getCommandName() {
    // Get the current IDC command class, and return its commandName.
    $idc_class = get_called_class();
    return $idc_class::$commandName;
  }

  /**
   * Returns the results of all steps (or a given step) executed so far.
   *
   * @param null $stepName
   *   The name of the step for which to retrieve input results.
   * @return mixed
   *   The results of the requested steps.
   */
  public function getStepResults($stepName = NULL) {
    if (!is_null($stepName)) {
      return $this->stepResults[$stepName];
    }
    return $this->stepResults;
  }

  /**
   * Sets the results (options chosen / data entered) for a given step.
   *
   * @param $stepName
   *   The name of the step for which to set the passed results.
   * @param $stepResults
   *   The results to set for the given step.
   * @return $this
   */
  public function setStepResults($stepName, $stepResults) {
    $this->stepResults[$stepName] = $stepResults;
    return $this;
  }

  /**
   * Checks whether the current IDC command has a next step to execute.
   *
   * @return bool
   *   TRUE, if there's a next step to execute, FALSE otherwise.
   */
  public function hasNextStep() {
    if (!is_null($this->nextStep) || ($this->currentStep < sizeof($this->commandSteps))) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Feeds the unicorns, as explicitly mentioned in the method name.
   *
   * Additionally, executes all the steps declared for the current IDC command.
   */
  public function feedTheUnicornsLittleTimmy() {
    // While there are next steps to execute, run all of them.
    while ($this->hasNextStep() && !$this->terminated) {
      try {
        $this->executeNextStep();
        // Increase currentStep counter for next iteration.
        $this->currentStep++;
      }
      catch (Exception $e) {
        // If something broke, terminate execution with a graceful movement.
        $this->terminate($e->getMessage());
      }
    }
    // No more steps. Run finishExecution() if process  hasn't been terminated.
    if (!$this->terminated) {
      $this->finishExecution();
    }
  }

  /**
   * Checks the name of the method tied to the next step of the IDC command.
   *
   * If there's a specific method set by the command, as a result of another
   * step, that one is returned, and the currentStep updated to reflect the
   * right value. Otherwise, the next method is returned based on the value of
   * the currentStep counter.
   *
   * @return string
   *   The name of the method that should be called to execute the next step.
   */
  public function getNextStepMethod() {
    // If there's a method specifically set as the next, it means the current
    // command tried to iterate back to a given step, so return that one.
    if (!is_null($this->nextStep)) {
      $nextStepMethod = $this->nextStep;
      // Set it to null so that it doesn't get executed endlessly, and set the
      // currentStep counter to the relevant index.
      $this->currentStep = array_search($nextStepMethod, $this->commandSteps);
      $this->setNextStep(NULL);
      return $nextStepMethod;
    }
    // No specific method set up, so proceed with the next in the list.
    else {
      return $this->commandSteps[$this->currentStep];
    }
  }

  /**
   * Returns the method name of the current step.
   *
   * @return string
   *   The method name of the current step.
   */
  public function getCurrentStepMethod() {
    return $this->commandSteps[$this->currentStep];
  }

  /**
   * Sets the method to execute for the next step.
   *
   * @param string $nextStep
   *   The name of the method to execute for the next step.
   * @return $this
   */
  private function setNextStep($nextStep) {
    $this->nextStep = $nextStep;
    return $this;
  }

  /**
   * Sets the method to execute for the next step.
   *
   * Wrapper around setNextStep() method, for command classes.
   *
   * @param string $stepName
   *   The name of the method to execute for the next step.
   * @return $this
   */
  protected function jumpToStep($stepName) {
    $this->setNextStep($stepName);
    return $this;
  }

  /**
   * Repeats the current step.
   *
   * Wrapper around setNextStep() method, to allow command classes to easily
   * repeat the current step.
   *
   * @return $this
   */
  protected function repeatStep() {
    $this->setNextStep($this->getCurrentStepMethod());
    return $this;
  }

  /**
   * Executes the next step of the current IDC command.
   *
   * This method works in 2 phases:
   *   1.- Gets the next step Step object, runs it, and fetches the results.
   *   2.- Once the results are available and stored, calls the same method
   *       again, telling the method that the step has been processed, in case
   *       that method decides to do any additional processing with the data, or
   *       to alter the execution flow in any way.
   */
  public function executeNextStep() {
    // Get name of the method that should return the next step. method is always
    // the same as the step name.
    $nextStepMethod = $this->getNextStepMethod();
    $stepHasMethod = TRUE;

    // Step defined a step object in the commandSteps() method. Get the object,
    // and set a flag to indicate there's no method to call for this step.
    if (isset($this->stepsInfo[$nextStepMethod]['step class'])) {
      $nextStep = $this->stepsInfo[$nextStepMethod]['step class'];
      $stepHasMethod = FALSE;
    }
    // Step should have a method. Double check it exists and call it to get the
    // step object.
    else {
      if (!method_exists($this, $nextStepMethod)) {
        $this->log('Step "' . $nextStepMethod . '" is defined, but no matching method was found. Skipping.', 'warning');
        return;
      }
      $nextStep = $this->{$nextStepMethod}();
    }

    // Execute step object, and store the results.
    if (!$this->terminated && $nextStep instanceof IDCStepBase) {
      $this->setStepResults($nextStepMethod, $this->executeStep($nextStep));
      // Call step method again (if any) with $processed = TRUE.
      if ($stepHasMethod) {
        $this->{$nextStepMethod}(TRUE);
      }
    }
  }

  /**
   * Executes the given step of a IDC command and return the input results.
   *
   * @param IDCStepBase $step
   * @return mixed
   *   The results (options chosen / or data entered by the user) for the given
   *   step after it's been executed.
   */
  public function executeStep(IDCStepBase $step) {
    $step->processStep();
    return $step->getInputResults();
  }

  /**
   * Terminates execution of the current command, without doing anything else.
   *
   * @param string $message
   *   The message to print to the command line, explaining the reason why the
   *   process was terminated.
   * @return $this
   */
  public function terminate($message = 'Execution failed. Terminating...') {
    // Show error message with the reason to terminate command.
    $this->log($message, 'error');
    $this->terminated = TRUE;
    return $this;
  }

  /**
   * Wrapper around drush_print(). Prints a message to the command line.
   *
   * @param $message
   *   The message to print. Obviously.
   * @return $this
   *
   * @see drush_print().
   */
  public function printMessage($message, $indent = 0, $handle = NULL, $newline = TRUE) {
    drush_print($message, $indent, $handle, $newline);
    return $this;
  }

  /**
   * Wrapper around drush_log(). Logs a status message on the command line.
   *
   * @param $message
   *   The message to print. Obviously.
   * @return $this
   *
   * @see drush_log().
   */
  public function log($message, $type = 'notice', $error = null) {
    drush_log($message, $type, $error);
    return $this;
  }

}
