<?php
/**
 * @file
 * Drush Interactive Commands Single Choice Step class.
 */

/**
 * Class IDCStepSingleChoice
 *
 * Defines an IDC Command Single-choice Step.
 */
class IDCStepSingleChoice extends IDCStepChoiceBase {

  /**
   * Constructs the Step object.
   *
   * @param $prompt
   * @param $options
   *
   * @see drush_choice();
   */
  public function __construct($prompt, $options) {
    $this->prompt = $prompt;
    $this->options = $options;
  }

  /**
   * Request input from the user in the appropriate manner and stores it.
   *
   * @see drush_choice();
   *
   * @return $this
   */
  public function requestInput() {
    $results = $this::singleChoice($this->getOptions(), $this->getPrompt(), '!value', array(), $this->choiceSettings);
    $this->setInputResults($results);
    return $this;
  }

}
