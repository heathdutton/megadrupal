<?php
/**
 * A result returned from MixedFractionParser after successfully parsing a
 * mixed fraction.
 *
 * © 2014-2015 RedBottle Design, LLC and Five Star Tool Company.
 * All rights reserved. This code is licensed under the GNU GPL v2 or later.
 *
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 */
class MixedFractionParseResult {
  /**
   * The type of fraction that was parsed (
   *
   * @var integer
   */
  protected $type;

  /**
   * The sign of the mixed fraction ('-' for negative; '+' or empty for
   * positive).
   *
   * @var string
   */
  protected $sign;

  /**
   * The whole number portion of the fraction; or 0, if the
   * fraction did not contain a whole number portion.
   *
   * @var integer
   */
  protected $wholeNumber;

  /**
   * The numerator portion of the fraction; or 0, if the
   * fraction did not contain a numerator portion.
   *
   * @var integer
   */
  protected $numerator;

  /**
   * The denominator portion of the fraction; or 1, if the
   * fraction did not contain a denominator portion.
   *
   * @var integer
   */
  protected $denominator;

  /**
   * The separator between the whole number and the fraction; or, an empty
   * string if the number did not contain both a whole number and a fraction.
   *
   * @var string
   */
  protected $wholeNumberSeparator;

  /**
   * The separator between the numerator and the denominator; or, an empty
   * string if the number did not contain a fraction.
   *
   * @var string
   */
  protected $fractionSeparator;

  /**
   * Constructor for MixedFractionParseResult.
   *
   * @param string $sign
   *   The sign of the mixed fraction ('-' for negative; '+' or empty for
   *   positive).
   *
   * @param integer $wholeNumber
   *   The whole number portion of the fraction. (If empty, defaults to 0).
   *
   * @param integer $numerator
   *   The numerator portion of the fraction. (If empty, defaults to 0).
   *
   * @param integer $denominator
   *   The denominator portion of the fraction. (If empty, defaults to 1).
   *
   * @param string $wholeNumberSeparator
   *   The separator between the whole number and the fraction.
   *
   * @param string $fractionSeparator
   *   The separator between the numerator and the denominator.
   */
  public function __construct($sign, $wholeNumber, $numerator, $denominator, $wholeNumberSeparator, $fractionSeparator) {
    if (!empty($sign) && !in_array($sign, array('+', '-'))) {
      throw new InvalidArgumentException("\$sign must be empty, '+'; or, '-'.");
    }

    if (!empty($wholeNumber) && ($wholeNumber < 0)) {
      throw new InvalidArgumentException("\$wholeNumber must be positive.");
    }

    if (!empty($numerator) && ($numerator < 0)) {
      throw new InvalidArgumentException("\$numerator must be positive.");
    }

    if (!empty($denominator) && ($denominator < 0)) {
      throw new InvalidArgumentException("\$denominator must be positive.");
    }

    if (!empty($wholeNumber) && !empty($numerator)) {
      $this->type = FRMI_FRACTION_TYPE_WHOLE_AND_FRACTION;
    }

    elseif (empty($wholeNumber) && !empty($numerator)) {
      $this->type = FRMI_FRACTION_TYPE_FRACTION_ONLY;
    }

    else {
      $this->type = FRMI_FRACTION_TYPE_WHOLE_ONLY;
    }

    $this->sign         = !empty($sign)         ? $sign         : '+';
    $this->wholeNumber  = !empty($wholeNumber)  ? $wholeNumber  : 0;
    $this->numerator    = !empty($numerator)    ? $numerator    : 0;
    $this->denominator  = !empty($denominator)  ? $denominator  : 1; // 0 would not be valid

    $this->wholeNumberSeparator = $wholeNumberSeparator;
    $this->fractionSeparator    = $fractionSeparator;
  }

  /**
   * Gets the type of mixed fraction, which can be any of the following:
   * - FRMI_FRACTION_TYPE_WHOLE_AND_FRACTION
   * - FRMI_FRACTION_TYPE_WHOLE_ONLY
   * - FRMI_FRACTION_TYPE_FRACTION_ONLY
   *
   * @return number
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Returns whether or not the fraction is negative.
   *
   * @return boolean
   */
  public function isNegative() {
    return ($this->sign == '-');
  }

  /**
   * Returns whether or not the fraction is positive.
   *
   * @return boolean
   */
  public function isPositive() {
    return (!$this->isNegative());
  }

  /**
   * Gets the sign of the number ('+' or '-').
   *
   * @return string
   */
  public function getSign() {
    return $this->sign;
  }

  /**
   * Returns the whole number portion of the fraction; or 0, if the fraction did
   * not contain a whole number portion.
   *
   * @return integer
   */
  public function getWholeNumber() {
    return $this->wholeNumber;
  }

  /**
   * Returns the numerator portion of the fraction; or 0, if the fraction did
   * not contain a numerator portion.
   *
   * @return integer
   */
  public function getNumerator() {
    return $this->numerator;
  }

  /**
   * Returns the denominator portion of the fraction; or 1, if the fraction did
   * not contain a denominator portion.
   *
   * @return integer
   */
  public function getDenominator() {
    return $this->denominator;
  }

  /**
   * Returns the separator between the whole number and the fraction; or, an
   * empty string if the number did not contain both a whole number and a
   * fraction.
   *
   * @return string
   */
  public function getWholeNumberSeparator() {
    return $this->wholeNumberSeparator;
  }

  /**
   * Returns the separator between the numerator and the denominator; or, an
   * empty string if the number did not contain a fraction.
   *
   * @var string
   */
  public function getFractionSeparator() {
    return $this->fractionSeparator;
  }
}