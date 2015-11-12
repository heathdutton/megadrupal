<?php
/**
 * MixedFraction class, which extends Fraction with useful methods for
 * converting into and out of mixed numbers and improper fractions.
 *
 * © 2014-2015 RedBottle Design, LLC and Five Star Tool Company.
 * All rights reserved. This code is licensed under the GNU GPL v2 or later.
 *
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 */
class MixedFraction extends Fraction {
  /**
   * The separators that are expected to separate whole number and fractional
   * parts in a mixed fraction string.
   *
   * @var array
   */
  protected $wholeNumberSeparators;

  /**
   * The separators that are expected to separate the numerator and denominator
   * in a mixed fraction string..
   *
   * @var array
   */
  protected $fractionSeparators;

  /**
   * Constructor for <code>MixedFraction</code>.
   *
   * @param mixed $mixedFraction
   *   The optional MixedFractionParseResult or mixed fraction string to parse
   *   and initialize this fraction with.
   *
   * @param mixed $wholeNumberSeparator
   *   An optional string or array argument for the separator(s) that are
   *   expected to separate whole number and fractional parts in a mixed
   *   fraction string. The default is to interpret both a hyphen ('-') and
   *   a space (' ') as a separator.
   *
   * @param mixed $fractionSeparator
   *   An optional string or array argument for the separator(s) that are
   *   expected to separate the numerator and denominator in a mixed
   *   fraction string. The default is a forward slash ('/').
   */
  public function __construct($mixedFraction = NULL, $numerator = NULL, $denominator = NULL,
                              $wholeNumberSeparator = NULL, $fractionSeparator = NULL) {
    if (!empty($mixedFraction) && (($numerator !== NULL) || ($denominator !== NULL))) {
      throw new InvalidArgumentException(
        '$numerator and $denominator must not be specified if $mixedFraction is specified.');
    }

    elseif ((($numerator !== NULL) && ($denominator === NULL)) ||
             (($numerator === NULL) && ($denominator !== NULL))) {
      throw new InvalidArgumentException(
        '$numerator and $denominator must be specified together.');
    }

    if (empty($wholeNumberSeparator)) {
      $wholeNumberSeparator = array('-', ' ');
    }

    if (empty($fractionSeparator)) {
      $fractionSeparator = array('/');
    }

    $this->wholeNumberSeparators = is_array($wholeNumberSeparator) ? $wholeNumberSeparator : array($wholeNumberSeparator);
    $this->fractionSeparators    = is_array($fractionSeparator)    ? $fractionSeparator    : array($fractionSeparator);

    if ($mixedFraction !== NULL) {
      parent::__construct(0, 1);

      if ($mixedFraction instanceof MixedFractionParseResult) {
        $this->fromParseResult($mixedFraction);
      }
      else {
        $this->fromMixedNumber($mixedFraction);
      }
    }
    else {
      parent::__construct($numerator, $denominator);
    }
  }

  /**
   * Return a mixed number string representation of the fraction.
   *
   * @param string $wholeNumberSeparator
   *   An optional argument for the separator to place between the whole number
   *   and fractional parts. The default is the first whole number separator
   *   that was passed to the constructor of this object.
   *
   * @param string $fractionSeparator
   *   An optional argument for the separator to place between the numerator
   *   and denominator. The default is the first fraction separator that was
   *   passed to the constructor of this object.
   *
   * @return string Returns a string with the whole number, whole number
   *   separator, numerator, fraction separator, and denominator.
   */
  public function toMixedNumber($wholeNumberSeparator = NULL, $fractionSeparator = NULL) {
    if (empty($wholeNumberSeparator)) {
      $wholeNumberSeparator = $this->wholeNumberSeparators[0];
    }

    if (empty($fractionSeparator)) {
      $fractionSeparator = $this->fractionSeparators[0];
    }

    $numerator   = $this->getNumerator();
    $denominator = $this->getDenominator();

    if ($numerator === 0) {
      $result = '0';
    }
    else {
      if ($numerator < 0) {
        $numerator  = -$numerator;
        $isNegative = TRUE;
      }
      else {
        $isNegative = FALSE;
      }

      if ($numerator < $denominator) {
        $result = $this->toString($fractionSeparator);
      }

      else {
        if (function_exists('bcdiv')) {
          $value        = bcdiv($numerator, $denominator);
          $wholeNumber  = $this->bcRound($value, 0);

          $newNumerator = bcsub($numerator, bcmul($wholeNumber, $denominator));
        }

        // If BCMath is not available, use normal PHP float division, multiplication, and rounding.
        else {
          $value        = $numerator / $denominator;
          $wholeNumber  = floor($value);

          $newNumerator = $numerator - ($wholeNumber * $denominator);
        }

        if ($isNegative) {
          $wholeNumber = -$wholeNumber;
        }

        $result = $wholeNumber;

        if ($newNumerator != 0) {
          $result .= $wholeNumberSeparator . $newNumerator . $fractionSeparator . $denominator;
        }
      }
    }

    return $result;
  }

  /**
   * Sets the numerator and denominator of this fraction based on parsing the
   * provided whole number string, optionally specifying the separator
   * between the whole number and the fractional piece, and the separator
   * between the numerator and denominator.
   *
   * @param string $value
   *   The number string to parse.
   *
   * @param mixed $wholeNumberSeparator
   *   An optional string or array argument for the separator(s) that are
   *   expected to separate whole number and fractional parts in a mixed
   *   fraction string. The default is the whole number separator(s)
   *   that was/were passed to the constructor of this object.
   *
   * @param mixed $fractionSeparator
   *   An optional string or array argument for the separator(s) that are
   *   expected to separate the numerator and denominator in a mixed
   *   fraction string. The default is the fraction separator(s)
   *   that was/were passed to the constructor of this object.
   */
  public function fromMixedNumber($value, $wholeNumberSeparator = NULL, $fractionSeparator = NULL) {
    if (!empty($value)) {
      if (empty($wholeNumberSeparator)) {
        $wholeNumberSeparator = $this->wholeNumberSeparators;
      }
      elseif (!is_array($wholeNumberSeparator)) {
        $wholeNumberSeparator = array($wholeNumberSeparator);
      }

      if (empty($fractionSeparator)) {
        $fractionSeparator = $this->fractionSeparators;
      }
      elseif (!is_array($fractionSeparator)) {
        $wholeNumberSeparator = array($fractionSeparator);
      }

      $parser       = MixedFractionParser::getInstance();
      $parsedValue  = $parser->parseFraction($value, $wholeNumberSeparator, $fractionSeparator);

      if (empty($parsedValue)) {
        $friendlyWholeNumberSeparators = implode(',', $wholeNumberSeparator);
        $friendlyFractionSeparators    = implode(',', $fractionSeparator);

        throw new UnexpectedValueException(
          "Not a valid mixed number fraction: '{$value}' " .
          "[wholeNumberSeparators='{$friendlyWholeNumberSeparators}', " .
          "fractionSeparators='{$friendlyFractionSeparators}']");
      }

      $this->fromParseResult($parsedValue);
    }
  }

  /**
   * Sets the numerator and denominator of this fraction based on interpreting
   * values from the provided MixedFractionParseResult object.
   *
   * @param MixedFractionParseResult $value
   *   The parse result object from which the value is to be interpreted.
   */
  public function fromParseResult(MixedFractionParseResult $value) {
    $wholeNumber  = $value->getWholeNumber();
    $numerator    = $value->getNumerator();
    $denominator  = $value->getDenominator();

    if (function_exists('bcdiv')) {
      $improperNumerator = bcadd($numerator, bcmul($wholeNumber, $denominator));
    }

    else {
      // If BCMath is not available, use normal PHP float division, multiplication, and rounding.
      $improperNumerator = $numerator + ($wholeNumber * $denominator);
    }

    // Handle sign correctly
    if ($value->isNegative()) {
      $improperNumerator = -$improperNumerator;
    }

    $this->setNumerator($improperNumerator);
    $this->setDenominator($denominator);
  }
}