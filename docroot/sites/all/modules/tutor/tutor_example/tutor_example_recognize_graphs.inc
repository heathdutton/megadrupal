<?php

/**
 * @file
 * A question plugin for practicing recognizing graphs for some standard
 * function types.
 */

class TutorExampleRecognizeGraphs extends TutorQuestion {
  function labelGet() {
    return ('Recognize function graphs');
  }

  function parametersGenerate() {
    $types = array(
      'linear' => tutor_math_rand(range(1, 4), TRUE), // Integer +- 1–4
      'proportional'=> tutor_math_rand(range(1, 4), TRUE), // Integer +- 1–4
      'power' => tutor_math_rand(array(.5, 1.5, 2, 3), TRUE),
      'exponential' => tutor_math_rand(array(.5, 1.5, 2), TRUE),
    );
    $seed = array_rand($types);
    $this->parameters = array(
      'type' => $seed,
      'a' => $types[$seed],
      'b' => tutor_math_rand(range(1, 4), TRUE), // Integer +- 1–4
    );
    return $this;
  }

  function buildQuestion(&$form) {
    parent::buildQuestion($form);

    $graph = new TutorCanvasCoordinatePlane('tutor-example-recognize-graphs', 400, 200);
    $graph->window = array(
      'x' => array('min' => -4, 'max' => 4, 'step' => 1),
      'y' => array('min' => -5, 'max' => 10, 'step' => 1),
    );
    switch ($this->parameters['type']) {
      case 'linear':
        $x_values = range(-10, 10, 0.5);
        foreach ($x_values as $x) {
          $y_values[] = $this->parameters['a'] * $x + $this->parameters['b'];
        }
        break;

      case 'proportional':
        $x_values = range(-10, 10, 0.5);
        foreach ($x_values as $x) {
          $y_values[] = $this->parameters['a'] * $x;
        }
        break;

      case 'power':
        // Check for root expressions
        if ($this->parameters['a'] != round($this->parameters['a'], 0)) {
          $x_values = range(0, 4, .2);
        }
        // Special case for negative exponents – break the curve into two parts.
        elseif ($this->parameters['a'] < 0) {
          $x_values = range(-4, -.1, .1);
          foreach ($x_values as $x) {
            $y_values[] = pow($x, $this->parameters['a']);
          }
          $graph->addDataSet('curve', $x_values, $y_values);
          unset($y_values);
          $x_values = range(.1, 4, .1);
        }
        else {
          $x_values = range(-4, 4, .2);
        }
        foreach ($x_values as $x) {
          $y_values[] = pow($x, $this->parameters['a']);
        }
        break;

      case 'exponential':
        $x_values = range(-10, 10, 0.5);
        foreach ($x_values as $x) {
          $y_values[] = pow(abs($this->parameters['a']), $x) * $this->parameters['b'] / 2;
        }
        break;
    }

    $graph->addDataSet('curve', $x_values, $y_values);
    $form['graph'] = array(
      '#markup' => $graph->getMarkup(),
    );
    $form['answer']['answer'] = array(
      '#type' => 'radios',
      '#title' => t('What kind of function does this graph show?'),
      '#required' => TRUE,
      '#weight' => 80,
      '#attributes' => array('class' => array('tutor-block')),
      '#options' => array(
        'linear' => t('A linear function'),
        'proportional' => t('A proportional (and also linear) function'),
        'power' => t('A power function'),
        'exponential' => t('An exponential function'),
        'other' => t('None of above, or there is not enough information to say'),
      ),
    );
    return $this;
  }

  function evaluateAnswer() {
    if ($this->answer == 'linear' && $this->parameters['type'] == 'proportional') {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_CLOSE, t('Have another look at the options.'));
    }
    else {
      $this->response = new TutorQuestionResponse($this->answer == $this->parameters['type']);
    }
    return $this;
  }
}
