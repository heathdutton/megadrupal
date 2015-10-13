<?php

/**
 * @file
 * A question plugin that creates an equation of the type c*a^kx = b.
 */

class MathExponentialEquation extends TutorQuestion {
  function labelGet() {
    return t('Solve exponential equations with logarithms');
  }

  function parametersGenerate() {
    $parameters = array(
      '@c' => tutor_math_rand(array(0.5, 1.5, 2, 2.5, 3), TRUE),
      '@a' => tutor_math_rand(range(5, 15), FALSE, array(10)) / 10,
      '@k' => tutor_math_rand(range(5, 15), FALSE, array(10)) / 10,
      '@b' => tutor_math_rand(range(5, 50), FALSE, array(10)) / 10,
    );
    // If the coefficient c is negative, then the result b must be negative too.
    if ($parameters['@c'] < 0) {
      $parameters['@b'] *= -1;
    }

    // Avoid having factors b and c equal, since it makes the equation trivial.
    if ($parameters['@b'] == $parameters['@c']) {
      $parameters['@b'] = $parameters['@b'] * 2;
    }

    $this->parameters = $parameters;
  }

  function buildQuestion(&$form) {
    parent::buildQuestion($form);

    $form['equation'] = array(
      '#markup' => t('@c &middot; @a<sup>@kx</sup> = @b <br />Solve for x using logarithms.', $this->parameters),
    );
    $form['answer']['answer']['#prefix'] = t('x = ');
    $form['answer']['answer']['#description'] = t("To use logarithms, type for example 'log(15)'.");

    return $this;
  }

  function evaluateAnswer() {
    // Check that the answer contains two log expressions. (There should
    // probably be three of them.)
    if (count(explode('log', $this->answer)) < 1) {
      $this->response = new TutorQuestionResponse(TUTOR_ANSWER_INVALID, t('Your answer seems to be on the wrong form.'));
      return $this;
    }

    // Verify the answer.
    $vars = array(
      'x' => tutor_math_evaluate($this->answer),
      'c' => $this->parameters['@c'],
      'a' => $this->parameters['@a'],
      'k' => $this->parameters['@k'],
    );
    if ($vars['x'] === FALSE) {
      // Evaluation of the expression has failed.
      return new TutorQuestionResponse(TUTOR_ANSWER_INVALID, t('Your answer seems to be on the wrong form.'));
    }
    else {
      // Verify that the expression for x actually results in the correct value.
      $expression = 'c * a ^ (k * x)';
      $this->response = new TutorQuestionResponse((tutor_math_evaluate($expression, $vars) == $this->parameters['@b']));
      return $this;
    }
  }
}
