<?php
/**
 * @file
 * Drush Interactive Commands Prompt Step class.
 */

/**
 * Class IDCStepPrompt
 *
 * Defines an IDC Command Prompt Step.
 */
class IDCStepPrompt extends IDCStepBase {

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
    $this->prompt = $prompt;
    $this->default_value = $default_value;
    $this->required = $required;
    $this->password = $password;
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
    $result = drush_prompt($this->getPrompt(), $this->default_value, $this->required, $this->password);
    $this->setInputResults($result);
    return $this;
  }
}
