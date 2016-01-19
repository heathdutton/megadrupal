<?php

/**
 * @file
 * A class describing a question used for multiplying binomials, such as
 * rewriting '(x + 2)(x + 3)' to 'x^2 + 5x + 6'.
 */

class TutorExampleMultiplyBinomials1 extends TutorQuestion {
  function labelGet() {
    return t('Multiply binomials 1');
  }

  function parametersGenerate() {
    // This question uses two constants; one in each parenthesis.
    $parameters = array(
      'a' => rand(2, 7),
      'b' => rand(2, 7),
    );

    $this->parameters = $parameters;
    return $this;
  }

  function buildQuestion(&$form) {
    parent::buildQuestion($form);
    // Add a simple markup field to present the question.
    $form['description'] = array(
      '#weight' => 10,
      '#type' => 'markup',
      '#markup' => t('Expand (x + @a)(x + @b).', array('@a' => $this->parameters['a'], '@b' => $this->parameters['b'])),
    );
    return $this;
  }

  function evaluateAnswer() {
    // Create new names for the variables, for easier access.
    foreach ($this->parameters as $name => $value) {
      ${$name} = $value;
    }

    // Verify that there is an x-square term in the answer:
    if (strstr($this->answer, 'x^2') == FALSE) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_INVALID);
      return $this;
    }

    // Verify that the anwer fits numerically. (This utilizes tutor_math.)
    $test_values = array('x' => rand(1000000, 2000000));
    $correct_answer = tutor_math_evaluate("(x + $a)(x + $b)", $test_values);
    $try = tutor_math_evaluate($this->answer, $test_values);
    if ($try == FALSE || $try != $correct_answer) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_WRONG);
      return $this;
    }

    // The answer seems to fit!
    $this->response = new TutorQuestionResponse(TUTOR_ANSWER_CORRECT);
    return $this;
  }
}
