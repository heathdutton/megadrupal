<?php

/**
 * @file
 * A class for recognizing that expressions like x^2 + 2x + 1 are squares.
 */

class TutorExampleIdentifyBinomialFactors extends TutorQuestion {
  function labelGet() {
    return t('Identify binomial squares');
  }

  function parametersGenerate() {
    // Allow random selection of the four different types of expressions to
    // build. Note that 'other' will be selected in 40% of the cases.
    $type = array('sum', 'difference', 'conjugate', 'other', 'other');
    // Allow picking to *different* random numbers 1–9.
    $values = range(1, 9);
    shuffle($values);

    $this->parameters = array(
      'type' => $type[array_rand($type)],
      'a' => array_pop($values),
      'b' => array_pop($values),
    );
    return $this;
  }

  function buildQuestion(&$form) {
    parent::buildQuestion($form);

    // Build the appropriate expression, to display in the question.
    switch ($this->parameters['type']) {
      case 'sum':
        $expression = 'x<sup>2</sup> + ' . ($this->parameters['a'] * 2) . 'x + ' . ($this->parameters['a'] * $this->parameters['a']);
        break;
      case 'difference':
        $expression = 'x<sup>2</sup> – ' . ($this->parameters['a'] * 2) . 'x + ' . ($this->parameters['a'] * $this->parameters['a']);
        break;
      case 'conjugate':
        $expression = 'x<sup>2</sup> – ' . ($this->parameters['a'] * $this->parameters['a']);
        break;
      case 'other':
        $expression = 'x<sup>2</sup> + ' . ($this->parameters['a'] + $this->parameters['b']) . 'x + ' . ($this->parameters['a'] * $this->parameters['b']);
        break;
    }

    // Add a simple markup field to present the question.
    $form['description'] = array(
      '#weight' => 10,
      '#type' => 'markup',
      '#markup' => t('How can !expression be factorized?', array('!expression' => $expression)),
    );

    // Replace the standard answer textfield with radiobuttons.
    $form['answer']['answer']['#type'] = 'radios';
    $form['answer']['answer']['#options'] = array(
      'sum' => t('Square of a sum, (x + a)<sup>2</sup>'),
      'difference' => t('Square of a difference, (x – a)<sup>2</sup>'),
      'conjugate' => t('A conjugate, (x + a)(x – a)'),
      'other' => t('None of the above'),
    );
    $form['answer']['answer']['#attributes']['class'] = array('tutor-block');
    $form['answer']['term'] = array(
      '#type' => 'textfield',
      '#weight' => 120,
      '#size' => 3,
      '#prefix' => t('a ='),
      '#attributes' => array('autocomplete' => array('off')),
    );
    $form['answer']['#attributes']['class'][] = 'tutor-inline';
    
    return $this;
  }

  function extractAnswer($form) {
    $this->answer = array(
      'type' => $form['answer']['answer']['#value'],
      'a' => $form['answer']['term']['#value'],
    );
    return $this;
  }

  function evaluateAnswer() {
    if ($this->parameters['type'] == 'other' && $this->answer['type'] == 'other') {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_CORRECT);
      return $this;
    }

    if ($this->answer['type'] == $this->parameters['type'] && empty($this->answer['a'])) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_INCOMPLETE, t("The term 'a' is missing."));
      return $this;
    }

    $this->response = new TutorQuestionResponse(($this->answer['type'] == $this->parameters['type'] && $this->answer['a'] == $this->parameters['a']));
    return $this;
  }
}
