<?php
/**
 * @file
 * Question class for consult module.
 */

class ConsultQuestion extends ConsultInterviewSubEntity {

  /**
   * Get question text value.
   *
   * @param bool $use_filter
   *   Whether or not to use the format filter.
   *
   * @return string
   *   The question text value.
   */
  public function getQuestionText($use_filter = TRUE) {
    $question_text = '';

    if (!empty($this->question['value'])) {
      $question_text = $this->question['value'];
    }

    if ($use_filter) {
      $question_text = check_markup($question_text, $this->getQuestionFormat());
    }

    return $question_text;
  }

  /**
   * Get question text format.
   *
   * @return string
   *   The question text format.
   */
  public function getQuestionFormat() {
    $question_format = 'plain_text';

    if (!empty($this->question['format'])) {
      $question_format = $this->question['format'];
    }

    return $question_format;
  }

  /**
   * Get the list of answers available for this question.
   *
   * @return array
   *   An array keyed by answer name and answer label.
   */
  public function getAnswers() {
    $answers = array();

    if (!empty($this->answers)) {
      $answers = $this->answers;
    }

    return $answers;
  }

  /**
   * Set the value for a given entity property.
   */
  public function setValue($key, $value) {
    $this->{$key} = $value;
  }

  /**
   * Get the edit path for this entity.
   *
   * @param string $op
   *   The operation to perform (eg edit or delete).
   *
   * @return string
   *   The Drupal path to edit this entity.
   */
  public function getOperationPath($op) {
    return 'admin/structure/consult/manage/' . $this->interview_name . '/question/' . $op . '/' . $this->identifier();
  }
}
