<?php

/**
 * @file
 * An example of how to write Tutor question classes.
 */

/**
 * The default class name is the CamelCase version of the question ID.
 */
class TutorExampleAddition extends TutorQuestion {
  /**
   * A short and concise label describing this question, wrapped in t().
   */
  function labelGet() {
    return t('Adding one-digit numbers');
  }

  /**
   * Generate parameters used when building and evaluating this question.
   *
   * This question will ask something like 'What is 1 + 2'. The two numbers are
   * stored as parameters. Note that the question object must be returned,
   */
  function parametersGenerate() {
    $this->parameters = array(
      'a' => rand(1, 9),
      'b' => rand(1, 9),
    );

    // Make sure to return the question object, as this method will be chained.
    return $this;
  }

  /**
   * Build the form that displays the question to the user.
   *
   * The question part of the form is normally put in the 'question' key of the
   * form array. Note that the form is passed by reference, and that the
   * question object must be returned.
   *
   * @param $form
   *   The form array that should be modified. Passed by reference.
   * @see TutorQuestion::buildQuestion()
   */
  function buildQuestion(&$form) {
    // Run the form through the parent class to get some standard components
    // for the form, such as a standard textfield for entering an answer.
    parent::buildQuestion($form);

    $form['question'] = array(
      '#type' => 'markup',
      '#markup' => t('What is @a + @b?', array('@a' => $this->parameters['a'], '@b' => $this->parameters['b'])),
    );

    // Again, the question object must be returned, to allow chaining.
    return $this;
  }

  /**
   * Extract the answer from the submitted form.
   *
   * This function should analyze the submitted form and store any answer in
   * $this->answer. As usual, the question object should be returned.
   */
  function extractAnswer($form) {
    // This is actually exactly what parent::extractAnswer does, but it is
    // written explicitly here as an example (rather than just calling the
    // parent).
    $this->answer = $form['answer']['answer']['#value'];

    // Again, the question object must be returned, to allow chaining.
    return $this;
  }

  /**
   * Compare the answer against the parameters to see if it is correct or not.
   */
  function evaluateAnswer() {
    // It is possible to do more than just right/wrong evaluation. First example
    // is to mark that the answer is invalid, in case the answer isn't numeric.
    if (!ctype_digit($this->answer)) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_INVALID);
    }
    elseif ($this->answer > ($this->parameters['a'] + $this->parameters['b'])) {
      // You can override the default messages for the responses, by passing a
      // new message as second parameter. Make sure to wrap in t().
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_WRONG, t('Too much!'));
    }
    elseif ($this->answer < ($this->parameters['a'] + $this->parameters['b'])) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_WRONG, t('Too little!'));
    }
    elseif ($this->answer == ($this->parameters['a'] + $this->parameters['b'])) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_CORRECT);
    }

    // The default value is a TUTOR_ANSWER_INCOMPLETE, so things will work even
    // if no response is set by this method.

    // Again, the question object must be returned, to allow chaining.
    return $this;
  }
}
