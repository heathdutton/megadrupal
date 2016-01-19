<?php

/**
 * @file
 * Classes and include files used by the Tutor Math module.
 */

/**
 * Helper function that splits an equation into right part and left part.
 *
 * @param $equation
 *   The equation to split into two expressions.
 * @return
 *   An array containing two strings – one for the left part and one for the
 *   right part of the equation.
 */
function tutor_math_equation_split($equation) {
  $parts = explode('=', $equation);
  // In case we don't have a valid equation, return something that is sure to
  // be evaluated as a non-valid equation (in this case "0 == 1").
  if (count($parts) != 2) {
    return array('0', '1');
  }

  return $parts;
}

/**
 * Inteface that must be implemented by all
 */
interface TutorMathEquationInterface {
  // Parameter values used for defining the equation, such as 2, 5 and -9 in
  // "2x + 5y = -9".
//  var $parameters = array();
  // The names of the variables used in the equation, such as x and y in
  // "2x + 5y = -9".
//  var $variableNames = array();

  /**
   * Method for evaluating if a given equation fits the relevant pattern.
   *
   * @param $equation
   *   A string with the equation to evaluate, such as "2x + 5y + 9 = 0" or
   *   "y = -2/5*x -9/5".
   * @return
   *   An object of the type TutorQuestionResponse, representing how well the
   *   given equation fits the relevant pattern.
   */
  function evaluate($equation);
}

/**
 * Some general equation functionality that can be extended by other classes.
 */
abstract class TutorMathEquation implements TutorMathEquationInterface {
  /**
   * Performs some general validations of the equation.
   *
   * Verifies that the required variables are set and valid, and checks that
   * the provided equation actually looks like an equation.
   *
   * @see TutorMathEquationInterface::evaluate()
   */
  function evaluate($equation) {
    // Verify that we have the required parameters set, as arrays.
    if (!isset($this->parameters) || !isset($this->variableNames) || !is_array($this->parameters) || !is_array($this->variableNames)) {
      return (new TutorQuestionResponse(TUTOR_ANSWER_INVALID, t('The equation setup seems invalid.')));
    }

    // Verify that we have something that looks like an equation.
    if (count(explode('=', $equation)) != 2) {
      return (new TutorQuestionResponse(TUTOR_ANSWER_INVALID, t('The expression is not a valid equation.')));
    }

    // All seems fine by initial checks.
    return new TutorQuestionResponse(TUTOR_ANSWER_CORRECT);
  }
}

/**
 * A class for evaluating linear equations with two variables.
 *
 * This class expects the equation to be described in the form Ax + By = C,
 * where A, B and C are the parameters describing the equation (while x and y
 * are the canonical names for the equation variables).
 */
class TutorMathEquationLinearTwoVars extends TutorMathEquation {
  function evaluate($equation) {
    // Perform basic checks.
    $result = parent::evaluate($equation);
    if ($result->response_type != TUTOR_ANSWER_CORRECT) {
      return $result;
    }

    // For increased code readability, create aliases for the equation
    // parameters and the variable names.
    $a = &$this->parameters[0];
    $b = &$this->parameters[1];
    $c = &$this->parameters[2];
    $x_name = &$this->variableNames[0];
    $y_name = &$this->variableNames[1];
    list($left, $right) = tutor_math_equation_split($equation);

    // Create preliminary initial values for x and y.
    $variables = array($x_name => rand(1000000, 2000000), $y_name => rand(1000000, 2000000));
    
    // In case both parameter $a and $b is zero, we don't have an equation with
    // any variables in it. Something's wrong.
    if ($a == 0 && $b == 0) {
      return new TutorQuestionResponse(TUTOR_ANSWER_CORRECT, t('The question seems weird, your answer will pass. But please tell the site administrator.'));
    }

    // Separate out the case where $b == 0, to avoid division by zero later on.
    if ($b == 0) {
      // Set variables that should give a correct evaluation of the equation.
      $variables[$x_name] = $c / $a;
      if (tutor_math_evaluate($left, $variables) != tutor_math_evaluate($right, $variables)) {
        return new TutorQuestionResponse(TUTOR_ANSWER_WRONG);
      }

      // Set variables that should give an incorrect evaluation of the equation.
      $variables[$x_name]++;
      if (tutor_math_evaluate($left, $variables) == tutor_math_evaluate($right, $variables)) {
        return new TutorQuestionResponse(TUTOR_ANSWER_WRONG, t('You seem to have the exact same thing on both sides of the equation.'));
      }
      
      // If we got this far, the equation is good.
      return $result;
    }
    
    // Find the y value corresponding to the randomly chosen x value.
    $variables[$y_name] = -1 * $a * $variables[$x_name] / $b + $c / $b;

    // Check that the submitted equation works for these variable values.
    if (tutor_math_evaluate($left, $variables) != tutor_math_evaluate($right, $variables)) {
      return new TutorQuestionResponse(TUTOR_ANSWER_WRONG);
    }

    // Check that the submitted equation doesn't work for other variable values.
    $variables[$y_name]++;
    if (tutor_math_evaluate($left, $variables) == tutor_math_evaluate($right, $variables)) {
        return new TutorQuestionResponse(TUTOR_ANSWER_WRONG, t('You seem to have the exact same thing on both sides of the equation.'));
    }

    // If we got this far, the equation is good.
    return $result;
  }
}
