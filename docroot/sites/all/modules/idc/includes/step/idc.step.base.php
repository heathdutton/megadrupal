<?php
/**
 * @file
 * Drush Interactive Commands Step class.
 */

/**
 * Class IDCStep
 *
 * Defines a IDC Command Step.
 */
abstract class IDCStepBase {

  /**
   * @var string The message to display to the user prompting for input.
   */
  protected $prompt;

  /**
   * @var array The options to show to the user.
   */
  protected $options;

  /**
   * @var mixed Option(s) chosen by the user.
   */
  protected $inputResults;

  /**
   * Constructs the Step object.
   */
  public function __construct($prompt) {
    $this->prompt = $prompt;
  }

  /**
   * Gets the question to show to the user when prompting him for input.
   *
   * @return string
   *   The question to show to the user when prompting him for input.
   */
  public function getPrompt() {
    return $this->prompt;
  }

  /**
   * Sets the question to show to the user when prompting him for input.
   *
   * @param $prompt
   *   The question to show to the user when prompting him for input.
   * @return $this
   */
  public function setPrompt($prompt) {
    $this->prompt = $prompt;
    return $this;
  }

  /**
   * Gets the options to display to the user.
   *
   * @return array
   *   The options to display to the user.
   */
  public function getOptions() {
    return $this->options;
  }

  /**
   * Sets the options to display to the user.
   *
   * @param $options
   *   The options to display to the user.
   * @return $this
   */
  public function setOptions($options) {
    $this->options = $options;
    return $this;
  }

  /**
   * Gets the choices selected by the user when requested for input.
   *
   * @return mixed
   */
  public function getInputResults() {
    return $this->inputResults;
  }

  /**
   * Sets the choices selected by the user when requested for input.
   *
   * @param $inputResults
   *   The choices selected by the user when requested for input.
   * @return mixed
   */
  public function setInputResults($inputResults) {
    $this->inputResults = $inputResults;
    return $this;
  }

  /**
   * Process the step. Main entry point of steps for code using step classes.
   *
   * This function can be used to add any pre-processing or post-processing
   * logic to the step, (e.g: before the input is requested to the user, and
   * after it's been requested but step execution hasn't finished yet).
   *
   * @return $this
   */
  public abstract function processStep();

  /**
   * Request input from the user in the appropriate manner and stores it.
   *
   * @return $this
   */
  public abstract function requestInput();

}
