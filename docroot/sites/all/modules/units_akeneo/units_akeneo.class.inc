<?php

/**
 * @file
 * Units Akeneo class converter.
 */

/**
 * Measure exception
 *
 * @author    Romain Monceau <romain@akeneo.com>
 * @copyright 2012 Akeneo SAS (http://www.akeneo.com)
 * @license   http://opensource.org/licenses/MIT MIT
 *
 * @abstract
 */
abstract class MeasureException extends \Exception {
}

/**
 * Unkown family measure exception
 *
 * @author    Romain Monceau <romain@akeneo.com>
 * @copyright 2012 Akeneo SAS (http://www.akeneo.com)
 * @license   http://opensource.org/licenses/MIT MIT
 */
class UnknownFamilyMeasureException extends MeasureException {
}

/**
 * Unkown measure exception
 *
 * @author    Romain Monceau <romain@akeneo.com>
 * @copyright 2012 Akeneo SAS (http://www.akeneo.com)
 * @license   http://opensource.org/licenses/MIT MIT
 */
class UnknownMeasureException extends MeasureException {
}

/**
 * Unkown Operator exception during a measure conversion
 *
 * @author    Romain Monceau <romain@akeneo.com>
 * @copyright 2012 Akeneo SAS (http://www.akeneo.com)
 * @license   http://opensource.org/licenses/MIT MIT
 */
class UnknownOperatorException extends MeasureException {
}

/**
 * Aims to convert measures.
 *
 * @author    Romain Monceau <romain@akeneo.com>
 * @copyright 2012 Akeneo SAS (http://www.akeneo.com)
 * @license   http://opensource.org/licenses/MIT MIT
 */
class MeasureConverter {
  /**
   * @var array
   */
  protected $config;

  /**
   * @var string
   */
  protected $family;

  /**
   * Constructor.
   *
   * @param array $config
   *   Configuration parameters.
   */
  public function __construct($config = array()) {
    $this->config = $config['measures_config'];
  }

  /**
   * Set a family for the converter.
   *
   * @param string $family
   *   Family.
   *
   * @return MeasureConverter
   *   Measure.
   *
   * @throws UnknownFamilyMeasureException
   */
  public function setFamily($family) {
    if (!isset($this->config[$family])) {
      throw new UnknownFamilyMeasureException();
    }
    $this->family = $family;

    return $this;
  }

  /**
   * Convert a value from a base measure to a final measure.
   *
   * @param string $base_unit
   *   Base unit for value.
   * @param string $final_unit
   *   Result unit for value.
   * @param double $value
   *   Value to convert.
   *
   * @return double
   *   Converted value.
   */
  public function convert($base_unit, $final_unit, $value) {
    $standard_value = $this->convertBaseToStandard($base_unit, $value);
    $result         = $this->convertStandardToResult($final_unit, $standard_value);

    return $result;
  }

  /**
   * Convert a value in a base unit to the standard unit.
   *
   * @param string $base_unit
   *   Base unit for value.
   * @param double $value
   *   Value to convert.
   *
   * @return double
   *   Value converted to standard.
   *
   * @throws UnknownOperatorException
   * @throws UnknownMeasureException
   */
  public function convertBaseToStandard($base_unit, $value) {
    if (!isset($this->config[$this->family]['units'][$base_unit])) {
      throw new UnknownMeasureException(
        sprintf(
          'Could not find metric unit "%s" in family "%s"',
          $base_unit,
          $this->family
        )
      );
    }

    $conversion_config = $this->config[$this->family]['units'][$base_unit]['convert'];
    $converted_value   = $value;

    foreach ($conversion_config as $operation) {
      foreach ($operation as $operator => $operand) {
        $converted_value = $this->applyOperation($converted_value, $operator, $operand);
      }
    }

    return $converted_value;
  }

  /**
   * Apply operation between value and operand by using operator.
   *
   * @param double $value
   *   Value to convert.
   * @param string $operator
   *   Operator to apply.
   * @param double $operand
   *   Operand to use.
   *
   * @return double
   *   Value converted.
   *
   * @throws UnknownOperatorException
   */
  protected function applyOperation($value, $operator, $operand) {
    $processed_value = $value;

    switch ($operator) {
      case 'div':
        if ($operand !== 0) {
          $processed_value = $processed_value / $operand;
        }
        break;

      case 'mul':
        $processed_value = $processed_value * $operand;
        break;

      case 'add':
        $processed_value = $processed_value + $operand;
        break;

      case 'sub':
        $processed_value = $processed_value - $operand;
        break;

      default:
        throw new UnknownOperatorException();
    }

    return $processed_value;
  }

  /**
   * Convert a value in a standard unit to a final unit.
   *
   * @param string $final_unit
   *   Final unit for value
   * @param double $value
   *   Value to convert
   *
   * @return double
   *   Value converted
   *
   * @throws UnknownOperatorException
   * @throws UnknownMeasureException
   */
  public function convertStandardToResult($final_unit, $value) {
    if (!isset($this->config[$this->family]['units'][$final_unit])) {
      throw new UnknownMeasureException(
        sprintf(
          'Could not find metric unit "%s" in family "%s"',
          $final_unit,
          $this->family
        )
      );
    }

    $conversion_config = $this->config[$this->family]['units'][$final_unit]['convert'];
    $converted_value   = $value;

    // Calculate result with conversion config.
    // (calculs must be reversed and operation inversed).
    foreach (array_reverse($conversion_config) as $operation) {
      foreach ($operation as $operator => $operand) {
        $converted_value = $this->applyReversedOperation($converted_value, $operator, $operand);
      }
    }

    return $converted_value;
  }

  /**
   * Apply reversed operation between value and operand by using operator.
   *
   * @param double $value
   *   Value to convert.
   * @param string $operator
   *   Operator to apply.
   * @param double $operand
   *   Operand to use.
   *
   * @return double
   *   Value converted.
   *
   * @throws UnknownOperatorException
   */
  protected function applyReversedOperation($value, $operator, $operand) {
    $processed_value = $value;
    switch ($operator) {
      case 'div':
        $processed_value = $processed_value * $operand;
        break;

      case 'mul':
        if ($operand !== 0) {
          $processed_value = $processed_value / $operand;
        }
        break;

      case 'add':
        $processed_value = $processed_value - $operand;
        break;

      case 'sub':
        $processed_value = $processed_value + $operand;
        break;

      default:
        throw new UnknownOperatorException();
    }

    return $processed_value;
  }
}
