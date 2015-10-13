<?php
/**
 * @file
 * Drush Interactive Commands Prompt CSV Step class.
 */

/**
 * Class IDCStepPromptCSV
 *
 * Defines an IDC Command Prompt Step.
 */
class IDCStepPromptCSVDecorator extends IDCStepBase {

  /**
   * @var \IDCStepPrompt the IDCStepPrompt command instance.
   */
  private $idcStepPromptInstance;

  /**
   * Constructs the Step object.
   *
   * @param $prompt
   * @param $default_value
   * @param $required
   * @param $password
   *
   * @see drush_prompt();
   */
  public function __construct($prompt, $default_value = NULL, $required = TRUE, $password = FALSE) {
    $this->idcStepPromptInstance = new IDCStepPrompt($prompt, $default_value, $required, $password);
  }

  /**
   * Process the step. Main entry point of steps for code using step classes.
   *
   * After the user has made his choice, the method stores it in the object.
   *
   * @see drush_prompt();
   */
  public function processStep() {
    $this->requestInput();
    return $this;
  }

  /**
   * Request input from the user in the appropriate manner and stores it.
   *
   * @return $this
   */
  public function requestInput() {
    $this->idcStepPromptInstance->requestInput();

    // Parse input as an CSV string and store it as an array.
    $this->setInputResults(array_map('trim', str_getcsv($this->idcStepPromptInstance->getInputResults())));
    return $this;
  }
}
