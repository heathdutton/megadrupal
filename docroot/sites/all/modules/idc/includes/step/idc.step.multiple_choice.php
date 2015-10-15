<?php
/**
 * @file
 * Drush Interactive Commands Multiple Choice Step class.
 */

/**
 * Class IDCStepMultipleChoice
 *
 * Defines a IDC Command Single-choice Step.
 */
class IDCStepMultipleChoice extends IDCStepChoiceBase {

  /**
   * Constructs the Step object.
   *
   * @param string $prompt
   * @param array $options
   * @param mixed $defaults
   *   This can take 3 forms:
   *    - FALSE: (Default) All options are unselected by default.
   *    - TRUE: All options are selected by default.
   *    - Array of $options keys to be selected by default.
   *
   * @see drush_choice_multiple();
   */
  public function __construct($prompt, $options, $defaults = FALSE) {
    $this->prompt = $prompt;
    $this->options = $options;
    $this->defaults = $defaults;

    // Enable CSV values.
    $this->choiceSettings['accept_csv'] = TRUE;
    // @see IDCStepChoiceBase::multipleChoice() method docblock.
    $this->partial_selections = array();
  }

  /**
   * Request input from the user in the appropriate manner and stores it.
   *
   * @see drush_choice_multiple();
   *
   * @return $this
   */
  public function requestInput() {
    $results = $this::multipleChoice($this->getOptions(), $this->defaults, $this->getPrompt(), '!value', '!value (selected)', 0, NULL, array(), $this->choiceSettings, $this->partial_selections);
    $this->setInputResults($results);
    return $this;
  }
}
