<?php

/**
 * @file
 * A class for recognizing that expressions like 4x^2 + 24x + 9 are squares.
 */

class TutorExampleIdentifyBinomialFactors2 extends TutorExampleIdentifyBinomialFactors {
  function labelGet() {
    return t('Identify binomial squares 2');
  }

  function parametersGenerate() {
    // Allow random selection of the four different types of expressions to
    // build. Note that 'other' will be selected in 40% of the cases.
    $type = array('sum', 'difference', 'conjugate', 'other', 'other');
    // Allow picking to *different* random numbers 1–9.
    $values = range(1, 9);
    shuffle($values);

    $parameters = array(
      'type' => $type[array_rand($type)],
      'a' => array_pop($values),
      'b' => array_pop($values),
    );

    // The 'a' and 'b' variables will be used to build the question. Let's add
    // a third parameter to use when for the 'other' case. And let's make sure
    // we don't accidentally get an even square.
    $c = rand(4, 50);
    while ($c == (2 * $parameters['a'] * $parameters['b'])) {
      $c = rand(4, 50);
    }
    $parameters['c'] = $c;
    $this->parameters = $parameters;
    return $this;
  }

  function buildQuestion(&$form) {
    parent::buildQuestion($form);
    foreach($this->parameters as $name => $value) {
      ${$name} = $value;
    }

    // Rebuild the expression to include the 'c' parameter as well.
    switch ($type) {
      case 'sum':
        $expression = ($a * $a) . 'x<sup>2</sup> + ' . (2 * $a * $b) . 'x + ' . ($b * $b);
        break;
      case 'difference':
        $expression = ($a * $a) . 'x<sup>2</sup> – ' . (2 * $a * $b) . 'x + ' . ($b * $b);
        break;
      case 'conjugate':
        $expression = ($a * $a) . 'x<sup>2</sup> – ' . ($b * $b);
        break;
      case 'other':
        $expression = ($a * $a) . 'x<sup>2</sup> + ' . $c . 'x + ' . ($b * $b);
        break;
    }
    $form['description']['#markup'] = t('How can !expression be factorized?', array('!expression' => $expression));

    $form['answer']['answer']['#options'] = array(
      'sum' => t('Square of a sum, (ax + b)<sup>2</sup>'),
      'difference' => t('Square of a difference, (ax – b)<sup>2</sup>'),
      'conjugate' => t('A conjugate, (ax + b)(ax – b)'),
      'other' => t('None of the above'),
    );

    $form['answer']['factor'] = $form['answer']['term'];
    $form['answer']['term']['#prefix'] = t('b =');
    
    return $this;
  }

  function extractAnswer($form) {
    $this->answer = array(
      'type' => $form['answer']['answer']['#value'],
      'a' => $form['answer']['factor']['#value'],
      'b' => $form['answer']['term']['#value'],
    );
    return $this;
  }

  function evaluateAnswer() {
    if ($this->parameters['type'] == 'other' && $this->answer['type'] == 'other') {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_CORRECT);
      return $this;
    }

    if ($this->answer['type'] == $this->parameters['type'] && (empty($this->answer['a']) || empty($this->answer['b']))) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_INCOMPLETE, t("The term 'a' or 'b' is missing."));
      return $this;
    }

    $this->response = new TutorQuestionResponse(($this->answer['type'] == $this->parameters['type'] && $this->answer['a'] == $this->parameters['a'] && $this->answer['b'] == $this->parameters['b']));
    return $this;
  }
}
