<?php

/**
 * @file
 * A class describing a question used for factorizing algebraic expressions,
 * such as turning '2x + 3xy + 4y + 5' into 'x(2 + 3y) + 4y + 5'.
 */

class TutorExampleFactorExpressions extends TutorQuestion {
  function labelGet() {
    return t('Factorize algebraic expression');
  }

  function parametersGenerate() {
    // This question will need six variables. Two of them are letters used as
    // variables inside the question, such as 'x' and 'y', and four are
    // coefficients/constants for each term in the original expression.

    // Select two random letters for variables in the question.
    $variable_names = array('x', 'p', 'r', 's', 'n', 'y');
    shuffle($variable_names);

    // Select four *different* random numbers.
    $values = range(2, 9);
    shuffle($values);

    // Build an array with all these values, and return it.
    $parameters = array(
      'main' => array_pop($variable_names),
      'second' => array_pop($variable_names),
      'a1' => array_pop($values),
      'a2' => array_pop($values),
      'a3' => array_pop($values),
      'a4' => array_pop($values),
    );

    $this->parameters = $parameters;
    return $this;
  }

  function buildQuestion(&$form) {
    parent::buildQuestion($form);
    // Create new names for the variables, for easier access.
    foreach ($this->parameters as $name => $value) {
      ${$name} = $value;
    }

    // Randomize the order of the terms in the question.
    $parts = array("$a1$main", "$a2$second$main", "$a3$second", "$a4");
    shuffle($parts);
    $expression = implode(' + ', $parts);

    // Insert a form element with plain markup for this question.
    $form['description'] = array(
      '#type' => 'markup',
      '#weight' => 10,
      '#markup' => t('As far as possible, rewrite as a factor of @main: %expression', array('@main' => $main, '%expression' => $expression)),
    );
    
    return $this;
  }

  function evaluateAnswer() {
    // Create new names for the variables, for easier access.
    foreach ($this->parameters as $name => $value) {
      ${$name} = $value;
    }

    // First, check that the answer is written as a factor of the main variable.
    tutor_strip($this->answer);
    if (strstr($this->answer, "$main($a2$second+$a1)") == FALSE && strstr($this->answer, "$main($a1+$a2$second)") == FALSE) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_WRONG);
      return $this;
    }

    // Evaluate the answer by inserting values to the variables, and calculating
    // the total expression.
    // (This evaluation utilizes the tutor_math module.)
    $test_values = array($main => rand(1000000, 2000000), $second => rand(1000000, 2000000));
    $correct_result = tutor_math_evaluate("$main($a2$second + $a1) + $a3$second + $a4", $test_values);
    $try = tutor_math_evaluate($this->answer, $test_values);
    if ($try == FALSE || $try != $correct_result) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_WRONG);
      return $this;
    }

    // The answer seems to fit!
    $this->response = new TutorQuestionResponse(TUTOR_ANSWER_CORRECT);
    return $this;
  }
}
