<?php
/**
 * A parser for handling all three common formats of mixed fractions:
 * - Whole number and fraction (1-1/2)
 * - Whole number only (1)
 * - Fraction only (1/2)
 *
 * � 2014-2015 RedBottle Design, LLC and Five Star Tool Company.
 * All rights reserved. This code is licensed under the GNU GPL v2 or later.
 *
 * @author Guy Paddock (guy.paddock)
 */
class MixedFractionParser {
  /**
   * The current instance of the parser.
   *
   * @var MixedFractionParser
   */
  protected static $instance;

  /**
   * Gets the current instance of the mixed fraction parser.
   * If there is no current instance, one is created before being returned.
   *
   * @return MixedFractionParser
   */
  public static function getInstance() {
    if (empty(self::$instance)) {
      self::$instance = new MixedFractionParser();
    }

    return self::$instance;
  }

  /**
   * Attempts to parse the specified fraction string into a
   * MixedFractionParseResult object, which conveys all of the
   * information that was obtained from parsing.
   *
   * @param string $fractionString
   *   The fraction to parse.
   *
   * @param array $wholeNumberSeparators
   *   An optional parameter for specifying each of the delimiters to expect in
   *   the fraction string to separate the whole number portion from the
   *   fraction portion. If not specified, the default is a space (' ') and
   *   hyphen ('-').
   *
   * @param array $fractionSeparators
   *   An optional parameter for specifying each of the delimiters to expect in
   *   the fraction string to separate the numerator portion from the
   *   denominator portion. If not specified, the default is a forward
   *   slash ('/').
   *
   * @return MixedFractionParseResult
   *   The result from parsing the fraction; or NULL, if the fraction could not
   *   be parsed.
   */
  public function parseFraction($fractionString, array $wholeNumberSeparators = NULL, array $fractionSeparators = NULL) {
    if (empty($wholeNumberSeparators)) {
      $wholeNumberSeparators = array(' ', '-');
    }

    if (empty($fractionSeparators)) {
      $fractionSeparators = array('/');
    }

    $sanitizedWholeNumberSeparators = array();

    foreach ($wholeNumberSeparators as $separator) {
      $sanitizedWholeNumberSeparators[] = preg_quote($separator, '/');
    }

    $sanitizedFractionSeparators = array();

    foreach ($fractionSeparators as $separator) {
      $sanitizedFractionSeparators[] = preg_quote($separator, '/');
    }

    /*
     * This regular expression should handle all three formats for a whole
     * number.
     *
     * The significance of matches is as follows:
     * - 1: The sign of the number (empty if positive, dash if negative).
     * - 3: The whole number portion.
     * - 4: The whole number separator that was parsed.
     * - 6: The numerator.
     * - 7: The fraction separator that was parsed.
     * - 8: The denominator.
     *
     * If at least 3, 6, and 8 are present, then the mixed fraction consists of
     * a whole number followed by a fraction.
     *
     * If only 6 and 8 are present (optionally including 1), the mixed fraction
     * consists only of a fraction.
     *
     * If only 3 is present (optionally including 1), the mixed fraction
     * consists only of a whole number without any fraction.
     */
    $fullRegex =
      sprintf(
        "/^(\+|-)?(([0-9]+)(%s)?)?(([0-9]+)(%s)([0-9]+))?$/U",
        implode('|', $sanitizedWholeNumberSeparators),
        implode('|', $sanitizedFractionSeparators));

    $matches = array();

    if (preg_match($fullRegex, $fractionString, $matches) !== 0) {
      $result =
        new MixedFractionParseResult(
          isset($matches[1]) ? $matches[1] : NULL,
          isset($matches[3]) ? $matches[3] : NULL,
          isset($matches[6]) ? $matches[6] : NULL,
          isset($matches[8]) ? $matches[8] : NULL,
          isset($matches[4]) ? $matches[4] : NULL,
          isset($matches[7]) ? $matches[7] : NULL);
    }
    else {
      $result = NULL;
    }

    return $result;
  }
}