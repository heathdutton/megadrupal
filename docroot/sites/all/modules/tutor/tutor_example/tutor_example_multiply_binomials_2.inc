<?php

/**
 * @file
 * A class describing a question used for multiplying binomials, and sking the
 * user to enter the polynomial coefficients rather than writing the full
 * expression. Note that this class extends a previously provided class, and
 * so inherits some methods from it.
 *
 * The benefits of extending an existing class in this particular case is
 * limited, but see this as an exmaple of *how* to extend rathern than *when*
 * to do it.
 */

class TutorExampleMultiplyBinomials2 extends TutorExampleMultiplyBinomials1 {
  function labelGet() {
    return t('Multiply binomials 2');
  }

  function parametersGenerate() {
    parent::parametersGenerate();
    return $this;
  }

  function buildQuestion(&$form) {
    // Inherit the question presentation from the parent class, but remove the
    // textfield for answering on a single line.
    parent::buildQuestion($form);
    unset($form['answer']['answer']);

    // Insert three separate smaller textfields, for entering the binomial
    // coefficients separately. Best tutor is to keep these inside the
    // ['answer'] container.
    $form['answer']['square'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#suffix' => 'x<sup>2</sup>',
      '#attributes' => array('autocomplete' => array('off')),
    );
    $form['answer']['lin'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#prefix' => '+',
      '#suffix' => 'x',
      '#attributes' => array('autocomplete' => array('off')),
    );
    $form['answer']['const'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#prefix' => '+',
      '#attributes' => array('autocomplete' => array('off')),
    );

    return $this;
  }

  function extractAnswer($form) {
    // The answer is stored in three sub parts of $form['answer']. Get these
    // and store them in an associative array.
    $entries = array('square', 'lin', 'const');
    foreach ($entries as $variable_name) {
      $answer[$variable_name] = $form['answer'][$variable_name]['#value'];
    }

    $this->answer = $answer;
    return $this;
  }

  function evaluateAnswer() {
    // Verify that all the entries are set.
    if (empty($this->answer['square']) || empty($this->answer['lin']) || empty($this->answer['const'])) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_INCOMPLETE);
      return $this;
    }
    
    // Check the answer (binomial coefficients) against the variables used to
    // build this question.
    if ($this->answer['square'] == 1 && $this->answer['lin'] == ($this->parameters['a'] + $this->parameters['b']) && $this->answer['const'] == ($this->parameters['a'] * $this->parameters['b'])) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_CORRECT);
      return $this;
    }
    else {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_WRONG);
      return $this;
    }
  }
}
