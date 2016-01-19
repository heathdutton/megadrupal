<?php

/**
 * @file
 * A class describing a question that generates a problem description which
 * should be turned into a linear equation.
 */

class TutorExampleEquationInText extends TutorQuestion {
  function labelGet() {
    return t('Interpret linear equation from text');
  }

  function parametersGenerate() {
    // Pick a random question type.
    $type = tutor_math_rand(array('age', 'weight', 'rectangle', 'fence'));

    // This question is rather complex to build, so each type will have to set
    // the variables by itself. Each question should set the following in the
    // $variables array:
    //   - @a: The parameter A in Ax + By = C.
    //   - @b: The parameter B in Ax + By = C.
    //   - @c: The parameter C in Ax + By = C.
    //   - question: A string ready to display for the user.
    //   - label1: A string used for promting the box for setting a name for
    //     variable 1.
    //   - label2: Label for variable 2 name.

    // Several of the question types use names, so let's build an array for
    // getting names easily.
    $names = tutor_names();

    switch ($type) {
      case 'age':
        $variables['@n1'] = array_pop($names);
        $variables['@n2'] = array_pop($names);
        $variables['@distractor'] = rand(2, 20);
        if (rand(1, 2) == 1) {
          $variables['@a'] = -1;
          $variables['@b'] = 1;
          $variables['@c'] = rand(2, 20);
          $variables['question'] = t('@n1 is @c years younger than @n2. @n2 weighs @distractor kg more than @n1. Write an equation that relates the ages of @n1 and @n2.', $variables);
        }
        else {
          $variables['@a'] = 1;
          $variables['@b'] = -1;
          $variables['@c'] = rand(2, 20);
          $variables['question'] = t('@n1 is @c years older than @n2. @n2 weighs @distractor kg less than @n1. Write an equation that relates the ages of @n1 and @n2.', $variables);
        }
        $variables['label1'] = t('Variable representing age of @n1', $variables);
        $variables['label2'] = t('Variable representing age of @n2', $variables);
        break;
      case 'weight':
        $variables['@n1'] = array_pop($names);
        $variables['@n2'] = array_pop($names);
        $variables['@distractor'] = rand(2, 20);
        if (rand(1, 2) == 1) {
          $variables['@a'] = -1;
          $variables['@b'] = 1;
          $variables['@c'] = rand(2, 20);
          $variables['question'] = t('@n1 is @distractor years younger than @n2. @n2 weighs @c kg more than @n1. Write an equation that relates the weights of @n1 and @n2.', $variables);
        }
        else {
          $variables['@a'] = 1;
          $variables['@b'] = -1;
          $variables['@c'] = rand(2, 20);
          $variables['question'] = t('@n1 is @distractor years older than @n2. @n2 weighs @c kg less than @n1. Write an equation that relates the weights of @n1 and @n2.', $variables);
        }
        $variables['label1'] = t('Variable representing weight of @n1', $variables);
        $variables['label2'] = t('Variable representing weight of @n2', $variables);
        break;
      case 'rectangle':
        $variables['@a'] = 2;
        $variables['@b'] = 2;
        $variables['@c'] = rand(2, 20) * 2;
        $variables['question'] = t('The circumference of a rectangle is @c cm. Write an equation that relates the height and the width of the rectangle.', $variables);
        $variables['label1'] = t('Variable for height', $variables);
        $variables['label2'] = t('Variable for width', $variables);
        break;
      case 'fence':
        $variables['@a'] = rand(20, 50) / 10;
        $variables['@b'] = -1;
        $variables['@c'] = $variables['@a'];
        $variables['@n1'] = array_pop($names);
        $variables['@distractor'] = rand(7, 23) / 10;
        $variables['question'] = t('@n1 is building a fence. The poles are @distractor meter high, and they are @a meters apart. Write an equation that relates the number of poles with the total length of the fence.', $variables);
        $variables['label1'] = t('Variable for number of poles', $variables);
        $variables['label2'] = t('Variable for total length', $variables);
        break;
    }
    $this->parameters = $variables;
    return $this;
  }

  function buildQuestion(&$form) {
    parent::buildQuestion($form);
    $form['description'] = array(
      '#type' => 'markup',
      '#markup' => $this->parameters['question'],
    );
    $form['answer']['n1'] = array(
      '#type' => 'textfield',
      '#title' => $this->parameters['label1'],
      '#size' => 1,
      '#maxlength' => 1,
      '#required' => TRUE,
    );
    $form['answer']['n2'] = array(
      '#type' => 'textfield',
      '#title' => $this->parameters['label2'],
      '#size' => 1,
      '#maxlength' => 1,
      '#required' => TRUE,
    );
    $form['answer']['answer']['#title'] = t('Equation');
    unset($form['answer']['answer']['#title_display']);
    
    return $this;
  }

  function extractAnswer($form) {
    $this->answer = array(
      'equation' => $form['answer']['answer']['#value'],
      'n1' => $form['answer']['n1']['#value'],
      'n2' => $form['answer']['n2']['#value'],
    );
    return $this;
  }

  function evaluateAnswer() {
    // Verify the equation.
    $equation = new TutorMathEquationLinearTwoVars();
    $equation->parameters = array(
      $this->parameters['@a'],
      $this->parameters['@b'],
      $this->parameters['@c'],
    );
    $equation->variableNames = array(
      $this->answer['n1'],
      $this->answer['n2'],
    );
    $this->response = $equation->evaluate($this->answer['equation']);
    return $this;
  }
}
